/* web_DDPServer.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */

/*
 * https://github.com/meteor/meteor/blob/devel/packages/ddp/DDP.md
 */

#include "parson.h"

#include "json.h"
#include "web_DDPServer__method.h"


#define WEB_DDP_SERVER_SIZE_THRESHOLD 100
#define WEB_DDP_SERVER_BIG_LIST_SLEEP 10000000

static corto_void web_DDPServer_connect(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *session = json_object_get_string(json, "session");
    const char *version = json_object_get_string(json, "version");
    web_DDPServer_Session ddpSession = NULL;

    /* TODO Future support for sessions */
    if (version && strcmp(version, "1")) {
        goto failed;
    } else {
        if (!session || !(ddpSession = corto_resolve(this->sessions, (corto_string)session))) {
            char *sessionId = web_random(17);
            ddpSession = web_DDPServer_SessionDeclareChild(this->sessions, sessionId);
            web_DDPServer_SessionDefine(ddpSession, conn);
            corto_setref(&conn->data, ddpSession);
            corto_dealloc(sessionId);
        } else {
            corto_setref(&ddpSession->conn, conn);
            corto_setref(&conn->data, ddpSession);
            corto_release(ddpSession);
        }

        web_DDPServer_Session_connected(ddpSession);
    }

    return;
failed:;
    web_DDPServer_Session_failed(conn);
}

static corto_void web_DDPServer_ping(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *id = json_object_get_string(json, "id");
    web_DDPServer_Session_pong(conn->data, (corto_string)id);
}

static corto_void web_DDPServer_sub(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Object *json) {
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

    web_DDPServer_Session_sub(conn->data, (corto_string)id, (corto_string)name, meta, value, scope);
}

static corto_void web_DDPServer_callMethod(web_DDPServer this, web_SockJsServer_Connection conn, corto_method m, corto_object instance, JSON_Array*params) {
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
            web_DDPServer_Session_error(conn->data, "complex arguments are not yet supported", NULL);
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

static corto_void web_DDPServer_method(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Value* json) {
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
            web_DDPServer_callMethod(this, conn, m, instance, params);
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
    web_DDPServer_Session_error(conn->data, reason, NULL);
    corto_dealloc(reason);
}

static corto_void web_DDPServer_api(web_DDPServer this, web_SockJsServer_UriRequest *conn, corto_string uri) {
    CORTO_UNUSED(this);

    corto_object o = NULL;

    /* Resolve object based on URI */
    o = corto_resolve(NULL, uri + 4);

    if (!o) {
        web_SockJsServer_UriRequest_setStatus(conn, 404);
        web_SockJsServer_UriRequest_write(conn, "404: resource not found\n");
    } else {
        corto_bool value = FALSE;
        corto_bool meta = FALSE;
        corto_bool scope = FALSE;

        /* Set correct content type */
        web_SockJsServer_UriRequest_setHeader(conn, "Content-Type", "application/json; charset=utf-8");

        /* Determine what to show */
        if (!strcmp(web_SockJsServer_UriRequest_getVar(conn, "value"), "true")) { value = TRUE; }
        if (!strcmp(web_SockJsServer_UriRequest_getVar(conn, "meta"), "true")) { meta = TRUE; }
        if (!strcmp(web_SockJsServer_UriRequest_getVar(conn, "scope"), "true")) { scope = TRUE; }
        if (!(value || meta || scope)) {
            value = TRUE;
        }

        /* Serialize object-value to JSON */
        struct corto_serializer_s serializer = corto_json_ser(CORTO_PRIVATE, CORTO_NOT, CORTO_SERIALIZER_TRACE_NEVER);

        if ((corto_typeof(o)->kind == CORTO_VOID) && (!meta && !scope)) {
            web_SockJsServer_UriRequest_write(conn, "\n");
        } else {
            corto_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, meta, value, scope, TRUE};
            corto_serialize(&serializer, o, &jsonData);
            web_SockJsServer_UriRequest_write(conn, jsonData.buffer);
            corto_dealloc(jsonData.buffer);
        }

        corto_release(o);
    }
}

/* $end */

/* ::corto::web::DDPServer::construct() */
corto_int16 _web_DDPServer_construct(web_DDPServer this) {
/* $begin(::corto::web::DDPServer::construct) */

    /* Set the handlers of the SockJsServer base */
    corto_setref(&web_SockJsServer(this)->onClose._parent.procedure, web_DDPServer_onClose_o);
    corto_setref(&web_SockJsServer(this)->onClose._parent.instance, this);

    corto_setref(&web_SockJsServer(this)->onMessage._parent.procedure, web_DDPServer_onMessage_o);
    corto_setref(&web_SockJsServer(this)->onMessage._parent.instance, this);

    corto_setref(&web_SockJsServer(this)->onUri._parent.procedure, web_DDPServer_onUri_o);
    corto_setref(&web_SockJsServer(this)->onUri._parent.instance, this);

    this->sessions = corto_voidCreateChild(this, "__sessions");

    return web_SockJsServer_construct(web_SockJsServer(this));
/* $end */
}

/* ::corto::web::DDPServer::getPublication(string name) */
web_DDPServer_Publication _web_DDPServer_getPublication(web_DDPServer this, corto_string name) {
/* $begin(::corto::web::DDPServer::getPublication) */

    /* Find matching publication */
    web_DDPServer_Publication pub = corto_lookup(this, name);
    if (!pub) {
        corto_object o = corto_resolve(NULL, name);
        if (o) {
            pub = web_DDPServer_PublicationDeclareChild(this, name);
            web_DDPServer_PublicationDefine(pub, o);
            corto_release(o);
        }
    }

    return pub;
/* $end */
}

/* ::corto::web::DDPServer::onClose(::corto::web::SockJsServer::Connection conn) */
corto_void _web_DDPServer_onClose(web_DDPServer this, web_SockJsServer_Connection conn) {
/* $begin(::corto::web::DDPServer::onClose) */
    CORTO_UNUSED(this);
    if (conn->data) {
        corto_delete(conn->data);
    }
/* $end */
}

/* ::corto::web::DDPServer::onMessage(::corto::web::SockJsServer::Connection conn,string message) */
corto_void _web_DDPServer_onMessage(web_DDPServer this, web_SockJsServer_Connection conn, corto_string message) {
/* $begin(::corto::web::DDPServer::onMessage) */
    CORTO_UNUSED(this);
    CORTO_UNUSED(conn);

    JSON_Value *root = json_parse_string(message);
    if (!root) {
        goto error;
    }

    if (json_value_get_type(root) == JSONObject) {
        JSON_Object *jsonObj = json_value_get_object(root);
        const char *msg = json_object_get_string(jsonObj, "msg");
        if (!strcmp(msg, "connect")) {
            web_DDPServer_connect(this, conn, jsonObj);
        } else if (!strcmp(msg, "ping")) {
            web_DDPServer_ping(this, conn, jsonObj);
        } else if (!strcmp(msg, "sub")) {
            web_DDPServer_sub(this, conn, jsonObj);
        } else if (!strcmp(msg, "method")) {
            web_DDPServer_method(this, conn, root);
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

/* ::corto::web::DDPServer::onUri(::corto::web::SockJsServer::UriRequest conn,string uri) */
corto_void _web_DDPServer_onUri(web_DDPServer this, web_SockJsServer_UriRequest *conn, corto_string uri) {
/* $begin(::corto::web::DDPServer::onUri) */
    CORTO_UNUSED(this);
    if (!memcmp(uri, "/api", 4)) {
        web_DDPServer_api(this, conn, uri);
    } else {
        web_SockJsServer_UriRequest_setStatus(conn, 404);
        web_SockJsServer_UriRequest_write(conn, "Invalid URI");
    }
/* $end */
}

/* ::corto::web::DDPServer::post(event e) */
/* $header(::corto::web::DDPServer::post) */
static corto_observableEvent web_DDPServer_findRelatedEvent(web_DDPServer this, corto_observableEvent e) {
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
corto_void _web_DDPServer_post(web_DDPServer this, corto_event e) {
/* $begin(::corto::web::DDPServer::post) */
    corto_uint32 size = 0;
    corto_observableEvent e2;

    corto_lock(this);
    /* Check if there is already another event in the queue for the same object.
     * if so, replace event with latest update. */
    if ((e2 = web_DDPServer_findRelatedEvent(this, corto_observableEvent(e)))) {
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

/* ::corto::web::DDPServer::run() */
corto_void _web_DDPServer_run(web_DDPServer this) {
/* $begin(::corto::web::DDPServer::run) */
    corto_event e;
    corto_ll events = corto_llNew();

    while (TRUE) {
        web_SockJsServer_poll(web_SockJsServer(this), 0);
        if (web_SockJsServer(this)->exiting) {
            break;
        }

        corto_lock(this);
        while ((e = corto_llTakeFirst(this->events))) {
            corto_llAppend(events, e);
        }
        corto_unlock(this);
        while ((e = corto_llTakeFirst(events))) {
            corto_event_handle(e);
            corto_release(e);
            web_SockJsServer_poll(web_SockJsServer(this), 1);
        }
    }
/* $end */
}
