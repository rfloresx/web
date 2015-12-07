/* $CORTO_GENERATED
 *
 * web_DDP.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "parson.h"

#include "json.h"
// #include "web_DDP__method.h"


#define WEB_DDP_SERVER_SIZE_THRESHOLD 100
#define WEB_DDP_SERVER_BIG_LIST_SLEEP 10000000

static corto_void web_DDP_connect(web_DDP this, web_HTTP_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *session = json_object_get_string(json, "session");
    const char *version = json_object_get_string(json, "version");
    web_DDP_Session ddpSession = NULL;

    /* TODO Future support for sessions */
    if (version && strcmp(version, "1")) {
        goto failed;
    } else {
        if (!session || !(ddpSession = corto_resolve(this->sessions, (corto_string)session))) {
            char *sessionId = web_random(17);
            ddpSession = web_DDP_SessionDeclareChild(this->sessions, sessionId);
            web_DDP_SessionDefine(ddpSession, conn);
            corto_setref(&conn->udata, ddpSession);
            corto_dealloc(sessionId);
        } else {
            corto_setref(&ddpSession->conn, conn);
            corto_setref(&conn->udata, ddpSession);
            corto_release(ddpSession);
        }

        web_DDP_Session_connected(ddpSession);
    }

    return;
failed:;
    web_DDP_Session_failed(conn);
}

static corto_void web_DDP_ping(web_DDP this, web_HTTP_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *id = json_object_get_string(json, "id");
    web_DDP_Session_pong(conn->udata, (corto_string)id);
}

static corto_void web_DDP_sub(web_DDP this, web_HTTP_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *id = json_object_get_string(json, "id");
    const char *name = json_object_get_string(json, "name");
    JSON_Array *params = json_object_get_array(json, "params");
    JSON_Object *cxparams = json_array_get_object(params, 0);
    corto_bool meta = FALSE, value = TRUE, scope = FALSE;
    int v;

    if ((v = json_object_get_boolean(cxparams, "meta")) != -1) {
        meta = v;
        value = FALSE;
    }
    if ((v = json_object_get_boolean(cxparams, "scope")) != -1) {
        scope = v;
        value = FALSE;
    }
    if ((v = json_object_get_boolean(cxparams, "value")) != -1) {
        value = v;
    }

    web_DDP_Session_sub(conn->udata, (corto_string)id, (corto_string)name, meta, value, scope);
}

static corto_void web_DDP_callMethod(web_DDP this, web_HTTP_Connection conn, corto_method m, corto_object instance, JSON_Array*params) {
    corto_int32 i = 1;

    /* Allocate buffer for arguments */
    void *buffer = alloca(corto_function(m)->size);
    void *ptr = buffer;

    /* Set this pointer in buffer */
    *(corto_object*)buffer = instance;
    ptr = CORTO_OFFSET(ptr, sizeof(corto_object));

    /* Parse parameters */
    corto_parameterseqForeach(corto_function(m)->parameters, p) {
        JSON_Value *arg = json_array_get_value(params, i);

        /* Determine the JSON type, call appropriate conversion */
        switch(json_value_get_type(arg)) {
        case JSONNull: {
            corto_object n = NULL;
            corto_convert(corto_primitive(corto_object_o), &n, corto_primitive(p.type), ptr);
            break;
        }
        case JSONString: {
            corto_string str = (char*)json_value_get_string(arg);
            corto_convert(corto_primitive(corto_string_o), &str, corto_primitive(p.type), ptr);
            break;
        }
        case JSONNumber: {
            double num = json_value_get_number(arg);
            corto_convert(corto_primitive(corto_float64_o), &num, corto_primitive(p.type), ptr);
            break;
        }
        case JSONObject:
        case JSONArray: {
            web_DDP_Session_error(conn->udata, "complex arguments are not yet supported", NULL);
            goto error;
        }
        case JSONBoolean: {
            corto_bool b = json_value_get_boolean(arg);
            corto_convert(corto_primitive(corto_bool_o), &b, corto_primitive(p.type), ptr);
            break;
        }
        default:
            break;
        }

        /* Progress parameter buffer to next parameter */
        ptr = CORTO_OFFSET(ptr, corto_type_sizeof(p.type));
        i++;
    }

    /* Call method */
    corto_callb(corto_function(m), NULL, buffer);

    return;
error:
    return;
}

static corto_void web_DDP_method(web_DDP this, web_HTTP_Connection conn, JSON_Value* json) {
    corto_string reason = NULL;
    corto_object instance = NULL;
    char *method = (char*)json_object_get_string(json_object(json), "method");
    JSON_Array* params = json_object_get_array(json_object(json), "params");

    if (!params) {
        corto_asprintf(&reason, "method %s: cannot find params", method);
        goto error;
    }

    /* Is the method an instance method? */
    if (method[0] == '.') {
        /* Instance is always the first parameter in the argument list */
        char* query = (char*)json_array_get_string(params, 0);
        if (!(instance = corto_resolve(NULL, query))) {
            corto_asprintf(&reason, "Instance '%s' for method '%s' not found!", query, method);
            goto error;
        }

        /* Resolve method on instance */
        corto_method m = corto_interface_resolveMethod(corto_typeof(instance), method + 1);
        if (m) {
            /* Method found, invoke it */
            web_DDP_callMethod(this, conn, m, instance, params);
        } else {
            corto_id id;
            corto_asprintf(&reason, "Method '%s' not found in type '%s' of instance '%s'!",
                method, corto_fullname(corto_typeof(instance), id), query);
            goto error;
        }

        corto_release(instance);
    } else {
        /* Ordinary methods not yet supported */
    }

    return;
error:
    web_DDP_Session_error(conn->udata, reason, NULL);
    corto_dealloc(reason);
}
/* $end */

web_DDP_Publication _web_DDP_getPublication(web_DDP this, corto_string name) {
/* $begin(corto/web/DDP/getPublication) */

    /* Find matching publication */
    web_DDP_Publication pub = corto_lookup(this, name);
    if (!pub && this->autoPublish) {
        corto_object o = corto_resolve(NULL, name);
        if (o) {
            pub = web_DDP_PublicationCreateChild(this, name, o);
            corto_release(o);
        }
    }

    return pub;
/* $end */
}

corto_void _web_DDP_onData(web_DDP this, web_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/DDP/onData) */
    JSON_Value *root = json_parse_string(msg);
    if (!root) {
        goto error;
    }

    if (json_value_get_type(root) == JSONObject) {
        JSON_Object *jsonObj = json_value_get_object(root);
        const char *msg = json_object_get_string(jsonObj, "msg");
        if (!strcmp(msg, "connect")) {
            web_DDP_connect(this, c, jsonObj);
        } else if (!strcmp(msg, "ping")) {
            web_DDP_ping(this, c, jsonObj);
        } else if (!strcmp(msg, "sub")) {
            web_DDP_sub(this, c, jsonObj);
        } else if (!strcmp(msg, "method")) {
            web_DDP_method(this, c, root);
        } else {
            goto msg_error;
        }
    } else {
        goto msg_error;
    }

msg_error:
    json_value_free(root);
error:;
/* $end */
}

corto_void _web_DDP_onPoll(web_DDP this) {
/* $begin(corto/web/DDP/onPoll) */
    corto_event e;
    corto_ll events = corto_llNew();

    /* Poll sockjs so it can send out heartbeats */
    web_SockJs_onPoll_v(this);

    /* Collect events */
    corto_lock(this);
    while ((e = corto_llTakeFirst(this->events))) {
        corto_llAppend(events, e);
    }
    corto_unlock(this);

    /* Handle events outside of lock */
    while ((e = corto_llTakeFirst(events))) {
        corto_event_handle(e);
        corto_release(e);

        /* If processing lots of events, ensure that SockJs gets a chance to
         * send out heartbeats */
        web_SockJs_onPoll_v(this);
    }
/* $end */
}

/* $header(corto/web/DDP/post) */
static corto_observableEvent web_DDP_findRelatedEvent(web_DDP this, corto_observableEvent e) {
    corto_iter iter = corto_llIter(this->events);
    corto_observableEvent e2;
    while ((corto_iterHasNext(&iter))) {
        e2 = corto_iterNext(&iter);
        if ((e2->me == e->me) &&
          (e2->observable == e->observable) &&
          (e2->source == e->source) &&
          (e2->observer == e->observer)) {
            return e2;
        }
    }
    return NULL;
}
/* $end */
corto_void _web_DDP_post(web_DDP this, corto_event e) {
/* $begin(corto/web/DDP/post) */
    corto_uint32 size = 0;
    corto_observableEvent e2;

    /* Append new event to queue */
    corto_lock(this);

    /* Check if there is already another event in the queue for the same object.
     * if so, replace event with latest update. */
    if ((e2 = web_DDP_findRelatedEvent(this, corto_observableEvent(e)))) {
        corto_llReplace(this->events, e2, e);
        corto_release(e2);
    } else {
        corto_llAppend(this->events, e);
    }
    size = corto_llSize(this->events);
    corto_unlock(this);

    /* If queue is getting big, slow down publisher */
    if (size > WEB_DDP_SERVER_SIZE_THRESHOLD) {
        corto_sleep(0, WEB_DDP_SERVER_BIG_LIST_SLEEP);
    }

/* $end */
}
