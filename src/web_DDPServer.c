/* web_DDPServer.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#define corto_web_LIB
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

static cx_void web_DDPServer_connect(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CX_UNUSED(this);
    const char *session = json_object_get_string(json, "session");
    const char *version = json_object_get_string(json, "version");
    web_DDPServer_Session ddpSession = NULL;

    /* TODO Future support for sessions */
    if (version && strcmp(version, "1")) {
        goto failed;
    } else {
        if (!session || !(ddpSession = cx_resolve(this->sessions, (cx_string)session))) {
            char *sessionId = web_random(17);
            ddpSession = web_DDPServer_SessionDeclareChild(this->sessions, sessionId);
            web_DDPServer_SessionDefine(ddpSession, conn);
            cx_setref(&conn->data, ddpSession);
            cx_dealloc(sessionId);
        } else {
            cx_setref(&ddpSession->conn, conn);
            cx_setref(&conn->data, ddpSession);
            cx_release(ddpSession);
        }

        web_DDPServer_Session_connected(ddpSession);
    }

    return;
failed:;
    web_DDPServer_Session_failed(conn);
}

static cx_void web_DDPServer_ping(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CX_UNUSED(this);
    const char *id = json_object_get_string(json, "id");
    web_DDPServer_Session_pong(conn->data, (cx_string)id);
}

static cx_void web_DDPServer_sub(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CX_UNUSED(this);
    const char *id = json_object_get_string(json, "id");
    const char *name = json_object_get_string(json, "name");
    JSON_Array *params = json_object_get_array(json, "params");
    JSON_Object *cxparams = json_array_get_object(params, 0);
    cx_bool meta = FALSE, value = TRUE, scope = FALSE;
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

    web_DDPServer_Session_sub(conn->data, (cx_string)id, (cx_string)name, meta, value, scope);
}

static cx_void web_DDPServer_callMethod(web_DDPServer this, web_SockJsServer_Connection conn, cx_method m, cx_object instance, JSON_Array*params) {
    cx_int32 i = 1;

    /* Allocate buffer for arguments */
    void *buffer = alloca(cx_function(m)->size);
    void *ptr = buffer;

    /* Set this pointer in buffer */
    *(cx_object*)buffer = instance; 
    ptr = CX_OFFSET(ptr, sizeof(cx_object));

    /* Parse parameters */
    cx_parameterseqForeach(cx_function(m)->parameters, p) {
        JSON_Value *arg = json_array_get_value(params, i);

        /* Determine the JSON type, call appropriate conversion */
        switch(json_value_get_type(arg)) {
        case JSONNull: {
            cx_object n = NULL;
            cx_convert(cx_primitive(cx_object_o), &n, cx_primitive(p.type), ptr);
            break;
        }
        case JSONString: {
            cx_string str = (char*)json_value_get_string(arg);
            cx_convert(cx_primitive(cx_string_o), &str, cx_primitive(p.type), ptr);
            break;
        }
        case JSONNumber: {
            double num = json_value_get_number(arg);
            cx_convert(cx_primitive(cx_float64_o), &num, cx_primitive(p.type), ptr);
            break;
        }
        case JSONObject:
        case JSONArray: {
            web_DDPServer_Session_error(conn->data, "complex arguments are not yet supported", NULL);
            goto error;
        }
        case JSONBoolean: {
            cx_bool b = json_value_get_boolean(arg);
            cx_convert(cx_primitive(cx_bool_o), &b, cx_primitive(p.type), ptr);
            break;
        }
        default:
            break;
        }

        /* Progress parameter buffer to next parameter */
        ptr = CX_OFFSET(ptr, cx_type_sizeof(p.type));
        i++;
    }

    /* Call method */
    cx_callb(cx_function(m), NULL, buffer);

    return;
error:
    return;
}

static cx_void web_DDPServer_method(web_DDPServer this, web_SockJsServer_Connection conn, JSON_Value* json) {
    cx_string reason = NULL;
    cx_object instance = NULL;
    char *method = (char*)json_object_get_string(json_object(json), "method");
    JSON_Array* params = json_object_get_array(json_object(json), "params");
    
    if (!params) {
        cx_asprintf(&reason, "method %s: cannot find params", method);
        goto error;
    }

    /* Is the method an instance method? */
    if (method[0] == '.') {
        /* Instance is always the first parameter in the argument list */
        char* query = (char*)json_array_get_string(params, 0);
        if (!(instance = cx_resolve(NULL, query))) {
            cx_asprintf(&reason, "Instance '%s' for method '%s' not found!", query, method);
            goto error;
        }

        /* Resolve method on instance */
        cx_method m = cx_interface_resolveMethod(cx_typeof(instance), method + 1);
        if (m) {
            /* Method found, invoke it */
            web_DDPServer_callMethod(this, conn, m, instance, params);
        } else {
            cx_id id;
            cx_asprintf(&reason, "Method '%s' not found in type '%s' of instance '%s'!", 
                method, cx_fullname(cx_typeof(instance), id), query);
            goto error;            
        }

        cx_release(instance);
    } else {
        /* Ordinary methods not yet supported */
    }

    return;
error:
    web_DDPServer_Session_error(conn->data, reason, NULL);
    cx_dealloc(reason);
}

static cx_void web_DDPServer_api(web_DDPServer this, web_SockJsServer_UriRequest *conn, cx_string uri) {
    CX_UNUSED(this);

    cx_object o = NULL;

    /* Resolve object based on URI */
    o = cx_resolve(NULL, uri + 4);

    if (!o) {
        web_SockJsServer_UriRequest_setStatus(conn, 404);
        web_SockJsServer_UriRequest_write(conn, "404: resource not found\n");
    } else {
        cx_bool value = FALSE;
        cx_bool meta = FALSE;
        cx_bool scope = FALSE;

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
        struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);

        if ((cx_typeof(o)->kind == CX_VOID) && (!meta && !scope)) {
            web_SockJsServer_UriRequest_write(conn, "\n");
        } else {
            cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, meta, value, scope, TRUE};
            cx_serialize(&serializer, o, &jsonData);
            web_SockJsServer_UriRequest_write(conn, jsonData.buffer);
            cx_dealloc(jsonData.buffer);
        }

        cx_release(o);
    }
}

/* $end */

/* ::corto::web::DDPServer::construct() */
cx_int16 _web_DDPServer_construct(web_DDPServer this) {
/* $begin(::corto::web::DDPServer::construct) */

    /* Set the handlers of the SockJsServer base */
    cx_setref(&web_SockJsServer(this)->onClose._parent.procedure, web_DDPServer_onClose_o);
    cx_setref(&web_SockJsServer(this)->onClose._parent.instance, this);

    cx_setref(&web_SockJsServer(this)->onMessage._parent.procedure, web_DDPServer_onMessage_o);
    cx_setref(&web_SockJsServer(this)->onMessage._parent.instance, this);

    cx_setref(&web_SockJsServer(this)->onUri._parent.procedure, web_DDPServer_onUri_o);
    cx_setref(&web_SockJsServer(this)->onUri._parent.instance, this);

    this->sessions = cx_voidCreateChild(this, "__sessions");

    return web_SockJsServer_construct(web_SockJsServer(this));
/* $end */
}

/* ::corto::web::DDPServer::getPublication(string name) */
web_DDPServer_Publication _web_DDPServer_getPublication(web_DDPServer this, cx_string name) {
/* $begin(::corto::web::DDPServer::getPublication) */

    /* Find matching publication */
    web_DDPServer_Publication pub = cx_lookup(this, name);
    if (!pub) {
        cx_object o = cx_resolve(NULL, name);
        if (o) {
            pub = web_DDPServer_PublicationDeclareChild(this, name);
            web_DDPServer_PublicationDefine(pub, o);
            cx_release(o);
        }
    }

    return pub;
/* $end */
}

/* ::corto::web::DDPServer::onClose(::corto::web::SockJsServer::Connection conn) */
cx_void _web_DDPServer_onClose(web_DDPServer this, web_SockJsServer_Connection conn) {
/* $begin(::corto::web::DDPServer::onClose) */
    CX_UNUSED(this);
    if (conn->data) {
        cx_delete(conn->data);
    }
/* $end */
}

/* ::corto::web::DDPServer::onMessage(::corto::web::SockJsServer::Connection conn,string message) */
cx_void _web_DDPServer_onMessage(web_DDPServer this, web_SockJsServer_Connection conn, cx_string message) {
/* $begin(::corto::web::DDPServer::onMessage) */
    CX_UNUSED(this);
    CX_UNUSED(conn);

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
cx_void _web_DDPServer_onUri(web_DDPServer this, web_SockJsServer_UriRequest *conn, cx_string uri) {
/* $begin(::corto::web::DDPServer::onUri) */
    CX_UNUSED(this);
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
static cx_observableEvent web_DDPServer_findRelatedEvent(web_DDPServer this, cx_observableEvent e) {
    cx_iter iter = cx_llIter(this->events);
    cx_observableEvent e2;
    while ((cx_iterHasNext(&iter))) {
        e2 = cx_iterNext(&iter);
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
cx_void _web_DDPServer_post(web_DDPServer this, cx_event e) {
/* $begin(::corto::web::DDPServer::post) */
    cx_uint32 size = 0;
    cx_observableEvent e2;

    cx_lock(this);
    /* Check if there is already another event in the queue for the same object.
     * if so, replace event with latest update. */
    if ((e2 = web_DDPServer_findRelatedEvent(this, cx_observableEvent(e)))) {
        cx_llReplace(this->events, e2, e);
        cx_release(e2);
    } else {
        cx_llAppend(this->events, e);
    }
    size = cx_llSize(this->events);
    cx_unlock(this);

    /* If queue is getting big, slow down publisher */
    if (size > WEB_DDP_SERVER_SIZE_THRESHOLD) {
        cx_sleep(0, WEB_DDP_SERVER_BIG_LIST_SLEEP);
    }
/* $end */
}

/* ::corto::web::DDPServer::run() */
cx_void _web_DDPServer_run(web_DDPServer this) {
/* $begin(::corto::web::DDPServer::run) */
    cx_event e;
    cx_ll events = cx_llNew();

    while (TRUE) {
        web_SockJsServer_poll(web_SockJsServer(this), 0);
        if (web_SockJsServer(this)->exiting) {
            break;
        }

        cx_lock(this);
        while ((e = cx_llTakeFirst(this->events))) {
            cx_llAppend(events, e);
        }
        cx_unlock(this);
        while ((e = cx_llTakeFirst(events))) {
            cx_event_handle(e);
            cx_release(e);
            web_SockJsServer_poll(web_SockJsServer(this), 1);
        }
    }
/* $end */
}
