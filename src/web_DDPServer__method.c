
#include "web_DDPServer__method.h"

/*
 * Gets the name of the collection from the method name without /insert,
 * /update, or /delete. The pointer returned must be freed.
 */
static char* web_DDPServer_methodCollection(const JSON_Object* obj) {
    const char* method = json_object_get_string(obj, "method");
    const char* lastSlash = strrchr(method, '/');
    size_t collectionLength = lastSlash - method;
    char* collection = cx_alloc(collectionLength + 1);
    strncat(collection, method, collectionLength);
    return collection;
}

cx_void web_DDPServer_methodInsert(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Value *json) {
    CX_UNUSED(_this);
    cx_assert(json_value_get_type(json), "Not a json object");
    JSON_Object* jsonObj = json_value_get_object(json);
    char* collection = web_DDPServer_methodCollection(jsonObj);
    JSON_Array* params = json_object_get_array(jsonObj, "params");
    cx_string reason = NULL;
    if (!params) {
        cx_asprintf(&reason, "cannot find params");
        goto error;
    }
    JSON_Value* selector = json_array_get_value(params, 0);
    if (!selector) {
        cx_asprintf(&reason, "params is empty");
        goto error;
    }
    // TODO seems a bit wasteful!
    char* serialization = json_serialize_to_string(selector);
    cx_object o = cx_json_deser(NULL, serialization);
    json_free_serialized_string(serialization);
    if (cx_define(o)) {
      cx_id fullname;
      cx_asprintf(&reason, "cannot define %s", cx_fullname(o, fullname));
      goto error;
    }
    if (!(o && cx_checkState(o, CX_DEFINED))) {
        cx_asprintf(&reason, "could not define");
        goto error;
    }
error:
    if (reason) {
        // TODO offending message
        web_DDPServer_Session_error(conn->data, reason, NULL);
        cx_dealloc(reason);
    }
    cx_dealloc(collection);
}

static cx_object web_DDPServer_methodInstancePrepare(JSON_Value* json, char** reason) {
    cx_assert(json_value_get_type(json), "Not a json object");
    JSON_Object* jsonObj = json_value_get_object(json);
    char* collection = web_DDPServer_methodCollection(jsonObj);
    cx_object scope = cx_resolve(NULL, collection);
    if (!scope) {
        cx_asprintf(reason, "cannot find scope %s", collection);
        goto error;
    }
    JSON_Array* params = json_object_get_array(jsonObj, "params");
    if (!params) {
        cx_asprintf(reason, "cannot find params");
        goto error;
    }
    JSON_Object* selector = json_array_get_object(params, 0);
    if (!selector) {
        cx_asprintf(reason, "params is empty");
        goto error;
    }
    const char* _id = json_object_get_string(selector, "_id");
    if (!_id) {
        cx_asprintf(reason, "cannot find _id field");
        goto error;
    }
    cx_object object = cx_resolve(NULL, (cx_string)_id);
    if (!object || cx_parentof(object) != scope) {
        cx_asprintf(reason, "cannot find %s in %s", _id, collection);
        goto error;
    }
    cx_dealloc(collection);
    return object;
error:
    cx_dealloc(collection);
    return NULL;
}

cx_void web_DDPServer_methodRemove(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Value* json) {
    CX_UNUSED(_this);
    cx_object object;
    char* reason = NULL;
    if (!(object = web_DDPServer_methodInstancePrepare(json, &reason))) {
        goto error;
    }
    cx_delete(object);
error:
    if (reason) {
        web_DDPServer_Session_error(conn->data, reason, NULL);
        // TODO offending message
        cx_dealloc(reason);
    }
}

static cx_bool web_DDPServer_methodUpdateSetNumber(void* dest, cx_type type, JSON_Value* value) {
    if (json_value_get_type(value) != JSONNumber) {
        cx_error("not deserializing JSON number");
        goto error;
    }
    cx_float64 number = json_value_get_number(value);
    cx_convert(cx_primitive(cx_float64_o), &number, cx_primitive(type), dest);
    return FALSE;
error:
    return TRUE;
}

static cx_bool web_DDPServer_methodUpdateSetText(void* dest, cx_type type, JSON_Value* value) {
    CX_UNUSED(type);
    const char *s = json_value_get_string(value);
    if (!s) {
        cx_error("not deserializing JSON text");
        goto error;
    }
    *(cx_string *)dest = strdup(s);
    return FALSE;
error:
    return TRUE;
}

static cx_bool web_DDPServer_methodUpdateSetPrimitive(void** dest, cx_type* type, cx_bool* reference, JSON_Value* value) {
    *reference = FALSE;
    cx_bool error = FALSE;
    switch (cx_primitive(*type)->kind) {
        case CX_INTEGER:
        case CX_UINTEGER:
        case CX_FLOAT:
            error = web_DDPServer_methodUpdateSetNumber(*dest, *type, value);
            break;
        case CX_TEXT:
            error = web_DDPServer_methodUpdateSetText(*dest, *type, value);
            break;
    }
    return error;
}

static cx_bool web_DDPServer_methodUpdateSetEach(cx_object object, const char* key, JSON_Value* value, char** errorReason) {
    char* keyCopy = strdup(key);
    const char* tok = strtok(keyCopy, ".");
    if (!tok || strcmp(tok, "value") != 0) {
        cx_asprintf(errorReason, "key \"%s\" does not set value", key);
        goto error;
    }

    void* dest = object;
    cx_type type = cx_typeof(object);
    cx_bool reference = FALSE;
    switch (type->kind) {
        case CX_PRIMITIVE:
            web_DDPServer_methodUpdateSetPrimitive(&dest, &type, &reference, value);
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
static cx_bool web_DDPServer_methodUpdateSet(cx_object object, JSON_Value* json, char** errorReason) {
    size_t i = 0, setCount = 0;
    JSON_Object* jsonObj = json_value_get_object(json);
    if (!jsonObj) {
        cx_asprintf(errorReason, "$set operation not a JSON object");
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

cx_void web_DDPServer_methodUpdate(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Value* json) {
    CX_UNUSED(_this);
    cx_object object;
    char* reason = NULL;
    if ((object = web_DDPServer_methodInstancePrepare(json, &reason)) == NULL) {
        goto error;
    }

    JSON_Array* params = json_object_get_array(json_value_get_object(json), "params");
    JSON_Object* modifiers = json_array_get_object(params, 1);
    if (!modifiers) {
        cx_asprintf(&reason, "cannot find modifiers");
        goto error;
    }
    size_t modifierCount = json_object_get_count(modifiers);
    size_t i;
    for (i = 0; i < modifierCount; i++) {
        const char* modifier = json_object_get_name(modifiers, i);
        cx_bool modifierError = FALSE;
        if (!strcmp(modifier, "$set")) {
            modifierError = web_DDPServer_methodUpdateSet(object,
                json_object_get_value(modifiers, modifier), &reason);
        } else if (strncmp(modifier, "$", 1)) {
            char* jsonStr = json_serialize_to_string(json_array_get_value(params, 1));
            cx_object o = cx_json_deser(object, jsonStr);
            json_free_serialized_string(jsonStr);
            if (!o) {
                cx_asprintf(&reason, "cannot update object");
                goto error;
            }
        } else {
          cx_asprintf(&reason, "modifier not supported: %s", modifier);
          goto error;
        }
        if (modifierError) {
            // TODO should we leave the object in e.g. invalid state?
            goto error;
        }
    }
    cx_update(object);
error:
    if (reason) {
        web_DDPServer_Session_error(conn->data, reason, NULL);
        // TODO offending message
        cx_dealloc(reason);
    }
}
