
#include "web_DDPServer__method.h"

/*
 * Gets the name of the collection from the method name without /insert,
 * /update, or /delete. The pointer returned must be freed.
 */
static char* web_DDPServer_methodCollection(const JSON_Object* obj) {
    const char* method = json_object_get_string(obj, "method");
    const char* lastSlash = strrchr(method, '/');
    size_t collectionLength = lastSlash - method;
    char* collection = corto_alloc(collectionLength + 1);
    strncat(collection, method, collectionLength);
    return collection;
}

corto_void web_DDPServer_methodInsert(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Value *json) {
    CORTO_UNUSED(this);
    corto_assert(json_value_get_type(json), "Not a json object");
    JSON_Object* jsonObj = json_value_get_object(json);
    char* collection = web_DDPServer_methodCollection(jsonObj);
    JSON_Array* params = json_object_get_array(jsonObj, "params");
    corto_string reason = NULL;
    if (!params) {
        corto_asprintf(&reason, "cannot find params");
        goto error;
    }
    JSON_Value* selector = json_array_get_value(params, 0);
    if (!selector) {
        corto_asprintf(&reason, "params is empty");
        goto error;
    }
    // TODO seems a bit wasteful!
    char* serialization = json_serialize_to_string(selector);
    corto_object o = corto_json_deser(serialization);
    json_free_serialized_string(serialization);
    if (corto_define(o)) {
      corto_id fullname;
      corto_asprintf(&reason, "cannot define %s", corto_fullname(o, fullname));
      goto error;
    }
    if (!(o && corto_checkState(o, CORTO_DEFINED))) {
        corto_asprintf(&reason, "could not define");
        goto error;
    }
error:
    if (reason) {
        // TODO offending message
        web_DDPServer_Session_error(conn->data, reason, NULL);
        corto_dealloc(reason);
    }
    corto_dealloc(collection);
}

static corto_object web_DDPServer_methodInstancePrepare(JSON_Value* json, char** reason) {
    corto_assert(json_value_get_type(json), "Not a json object");
    JSON_Object* jsonObj = json_value_get_object(json);
    char* collection = web_DDPServer_methodCollection(jsonObj);
    corto_object scope = corto_resolve(NULL, collection);
    if (!scope) {
        corto_asprintf(reason, "cannot find scope %s", collection);
        goto error;
    }
    JSON_Array* params = json_object_get_array(jsonObj, "params");
    if (!params) {
        corto_asprintf(reason, "cannot find params");
        goto error;
    }
    JSON_Object* selector = json_array_get_object(params, 0);
    if (!selector) {
        corto_asprintf(reason, "params is empty");
        goto error;
    }
    const char* _id = json_object_get_string(selector, "_id");
    if (!_id) {
        corto_asprintf(reason, "cannot find _id field");
        goto error;
    }
    corto_object object = corto_resolve(NULL, (corto_string)_id);
    if (!object || corto_parentof(object) != scope) {
        corto_asprintf(reason, "cannot find %s in %s", _id, collection);
        goto error;
    }
    corto_dealloc(collection);
    return object;
error:
    corto_dealloc(collection);
    return NULL;
}

corto_void web_DDPServer_methodRemove(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Value* json) {
    CORTO_UNUSED(this);
    corto_object object;
    char* reason = NULL;
    if (!(object = web_DDPServer_methodInstancePrepare(json, &reason))) {
        goto error;
    }
    corto_delete(object);
error:
    if (reason) {
        web_DDPServer_Session_error(conn->data, reason, NULL);
        // TODO offending message
        corto_dealloc(reason);
    }
}

static corto_bool web_DDPServer_methodUpdateSetNumber(void* dest, corto_type type, JSON_Value* value) {
    if (json_value_get_type(value) != JSONNumber) {
        corto_error("not deserializing JSON number");
        goto error;
    }
    corto_float64 number = json_value_get_number(value);
    corto_convert(corto_primitive(corto_float64_o), &number, corto_primitive(type), dest);
    return FALSE;
error:
    return TRUE;
}

static corto_bool web_DDPServer_methodUpdateSetText(void* dest, corto_type type, JSON_Value* value) {
    CORTO_UNUSED(type);
    const char *s = json_value_get_string(value);
    if (!s) {
        corto_error("not deserializing JSON text");
        goto error;
    }
    *(corto_string *)dest = strdup(s);
    return FALSE;
error:
    return TRUE;
}

static corto_bool web_DDPServer_methodUpdateSetPrimitive(void** dest, corto_type* type, corto_bool* reference, JSON_Value* value) {
    *reference = FALSE;
    corto_bool error = FALSE;
    switch (corto_primitive(*type)->kind) {
        case CORTO_INTEGER:
        case CORTO_UINTEGER:
        case CORTO_FLOAT:
            error = web_DDPServer_methodUpdateSetNumber(*dest, *type, value);
            break;
        case CORTO_TEXT:
            error = web_DDPServer_methodUpdateSetText(*dest, *type, value);
            break;
        default:
            break;
    }
    return error;
}

static corto_bool web_DDPServer_methodUpdateSetEach(corto_object object, const char* key, JSON_Value* value, char** errorReason) {
    char* keyCopy = strdup(key);
    const char* tok = strtok(keyCopy, ".");
    if (!tok || strcmp(tok, "value") != 0) {
        corto_asprintf(errorReason, "key \"%s\" does not set value", key);
        goto error;
    }

    void* dest = object;
    corto_type type = corto_typeof(object);
    corto_bool reference = FALSE;
    switch (type->kind) {
        case CORTO_PRIMITIVE:
            web_DDPServer_methodUpdateSetPrimitive(&dest, &type, &reference, value);
            break;
        default:
            break;
    }
    free(keyCopy);
    return FALSE;
error:
    free(keyCopy);
    return TRUE;
}

/*
 * json - the value to the key $set
 */
static corto_bool web_DDPServer_methodUpdateSet(corto_object object, JSON_Value* json, char** errorReason) {
    size_t i = 0, setCount = 0;
    JSON_Object* jsonObj = json_value_get_object(json);
    if (!jsonObj) {
        corto_asprintf(errorReason, "$set operation not a JSON object");
        goto error;
    }
    setCount = json_object_get_count(jsonObj);
    for (i = 0; i < setCount; i++) {
        const char* key = json_object_get_name(jsonObj, i);
        JSON_Value* value = json_object_get_value(jsonObj, key);
        if (web_DDPServer_methodUpdateSetEach(object, key, value, errorReason)) {
            goto error;
        }
    }
    return FALSE;
error:
    return TRUE;
}

corto_void web_DDPServer_methodUpdate(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Value* json) {
    CORTO_UNUSED(this);
    corto_object object;
    char* reason = NULL;
    if ((object = web_DDPServer_methodInstancePrepare(json, &reason)) == NULL) {
        goto error;
    }

    JSON_Array* params = json_object_get_array(json_value_get_object(json), "params");
    JSON_Object* modifiers = json_array_get_object(params, 1);
    if (!modifiers) {
        corto_asprintf(&reason, "cannot find modifiers");
        goto error;
    }
    size_t modifierCount = json_object_get_count(modifiers);
    size_t i;
    for (i = 0; i < modifierCount; i++) {
        const char* modifier = json_object_get_name(modifiers, i);
        corto_bool modifierError = FALSE;
        if (!strcmp(modifier, "$set")) {
            modifierError = web_DDPServer_methodUpdateSet(object,
                json_object_get_value(modifiers, modifier), &reason);
        } else if (strncmp(modifier, "$", 1)) {
            char* jsonStr = json_serialize_to_string(json_array_get_value(params, 1));
            corto_object o = corto_json_deser(jsonStr);
            json_free_serialized_string(jsonStr);
            if (!o) {
                corto_asprintf(&reason, "cannot update object");
                goto error;
            }
        } else {
          corto_asprintf(&reason, "modifier not supported: %s", modifier);
          goto error;
        }
        if (modifierError) {
            // TODO should we leave the object in e.g. invalid state?
            goto error;
        }
    }
    corto_update(object);
error:
    if (reason) {
        web_DDPServer_Session_error(conn->data, reason, NULL);
        // TODO offending message
        corto_dealloc(reason);
    }
}
