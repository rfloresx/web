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

static cx_void web_DDPServer_connect(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CX_UNUSED(_this);
    const char *session = json_object_get_string(json, "session");
    const char *version = json_object_get_string(json, "version");
    web_DDPServer_Session ddpSession = NULL;

    /* TODO Future support for sessions */
    if (version && strcmp(version, "1")) {
        goto failed;
    } else {
        if (!session || !(ddpSession = cx_resolve(_this->sessions, (cx_string)session))) {
            char *sessionId = web_random(17);
            ddpSession = web_DDPServer_Session__declareChild(_this->sessions, sessionId);
            web_DDPServer_Session__define(ddpSession, conn);
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

static cx_void web_DDPServer_ping(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CX_UNUSED(_this);
    const char *id = json_object_get_string(json, "id");
    web_DDPServer_Session_pong(conn->data, (cx_string)id);
}

static cx_void web_DDPServer_sub(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CX_UNUSED(_this);
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

static int strcmplast(const char* str1, const char* str2) {
    return strcmp(str1 + strlen(str1) - strlen(str2), str2);
}

static cx_void web_DDPServer_method(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Value* json) {
    const char *method = json_object_get_string(json_object(json), "method");
    if (!strcmplast(method, "/insert")) {
        web_DDPServer_methodInsert(_this, conn, json);
    } else if (!strcmplast(method, "/update")) {
        web_DDPServer_methodUpdate(_this, conn, json);
    } else if (!strcmplast(method, "/remove")) {
        web_DDPServer_methodRemove(_this, conn, json);
    } else {
        cx_warning("received unrecognized DDP method: %s", method);
        // TODO offending message
        char* reason = NULL;
        cx_asprintf(&reason, "unrecognized method \"%s\"", method);
        web_DDPServer_Session_error(conn->data, reason, NULL);
        cx_release(reason);
    }
}

static cx_void web_DDPServer_api(web_DDPServer _this, web_SockJsServer_UriRequest *conn, cx_string uri) {
    CX_UNUSED(_this);

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
cx_int16 _web_DDPServer_construct(web_DDPServer _this) {
/* $begin(::corto::web::DDPServer::construct) */

    /* Set the handlers of the SockJsServer base */
    cx_setref(&web_SockJsServer(_this)->onClose._parent.procedure, web_DDPServer_onClose_o);
    cx_setref(&web_SockJsServer(_this)->onClose._parent.instance, _this);

    cx_setref(&web_SockJsServer(_this)->onMessage._parent.procedure, web_DDPServer_onMessage_o);
    cx_setref(&web_SockJsServer(_this)->onMessage._parent.instance, _this);

    cx_setref(&web_SockJsServer(_this)->onUri._parent.procedure, web_DDPServer_onUri_o);
    cx_setref(&web_SockJsServer(_this)->onUri._parent.instance, _this);

    _this->sessions = cx_void__createChild(_this, "__sessions");

    return web_SockJsServer_construct(web_SockJsServer(_this));
/* $end */
}

/* ::corto::web::DDPServer::getPublication(string name) */
web_DDPServer_Publication _web_DDPServer_getPublication(web_DDPServer _this, cx_string name) {
/* $begin(::corto::web::DDPServer::getPublication) */

    /* Find matching publication */
    web_DDPServer_Publication pub = cx_lookup(_this, name);
    if (!pub) {
        cx_object o = cx_resolve(NULL, name);
        if (o) {
            pub = web_DDPServer_Publication__declareChild(_this, name);
            web_DDPServer_Publication__define(pub, o);
            cx_release(o);
        }
    }

    return pub;
/* $end */
}

/* ::corto::web::DDPServer::onClose(::corto::web::SockJsServer::Connection conn) */
cx_void _web_DDPServer_onClose(web_DDPServer _this, web_SockJsServer_Connection conn) {
/* $begin(::corto::web::DDPServer::onClose) */
    CX_UNUSED(_this);
    if (conn->data) {
        cx_delete(conn->data);
    }
/* $end */
}

/* ::corto::web::DDPServer::onMessage(::corto::web::SockJsServer::Connection conn,string message) */
cx_void _web_DDPServer_onMessage(web_DDPServer _this, web_SockJsServer_Connection conn, cx_string message) {
/* $begin(::corto::web::DDPServer::onMessage) */
    CX_UNUSED(_this);
    CX_UNUSED(conn);

    JSON_Value *root = json_parse_string(message);
    if (!root) {
        goto error;
    }

    if (json_value_get_type(root) == JSONObject) {
        JSON_Object *jsonObj = json_value_get_object(root);
        const char *msg = json_object_get_string(jsonObj, "msg");
        if (!strcmp(msg, "connect")) {
            web_DDPServer_connect(_this, conn, jsonObj);
        } else if (!strcmp(msg, "ping")) {
            web_DDPServer_ping(_this, conn, jsonObj);
        } else if (!strcmp(msg, "sub")) {
            web_DDPServer_sub(_this, conn, jsonObj);
        } else if (!strcmp(msg, "method")) {
            web_DDPServer_method(_this, conn, root);
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
cx_void _web_DDPServer_onUri(web_DDPServer _this, web_SockJsServer_UriRequest *conn, cx_string uri) {
/* $begin(::corto::web::DDPServer::onUri) */
    CX_UNUSED(_this);
    if (!memcmp(uri, "/api", 4)) {
        web_DDPServer_api(_this, conn, uri);
    } else {
        web_SockJsServer_UriRequest_setStatus(conn, 404);
        web_SockJsServer_UriRequest_write(conn, "Invalid URI");
    }
/* $end */
}

/* ::corto::web::DDPServer::post(event e) */
/* $header(::corto::web::DDPServer::post) */
static cx_observableEvent web_DDPServer_findRelatedEvent(web_DDPServer _this, cx_observableEvent e) {
    cx_iter iter = cx_llIter(_this->events);
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
cx_void _web_DDPServer_post(web_DDPServer _this, cx_event e) {
/* $begin(::corto::web::DDPServer::post) */
    cx_uint32 size = 0;
    cx_observableEvent e2;

    cx_lock(_this);
    /* Check if there is already another event in the queue for the same object.
     * if so, replace event with latest update. */
    if ((e2 = web_DDPServer_findRelatedEvent(_this, cx_observableEvent(e)))) {
        cx_llReplace(_this->events, e2, e);
        cx_release(e2);
    } else {
        cx_llAppend(_this->events, e);
    }
    size = cx_llSize(_this->events);
    cx_unlock(_this);

    /* If queue is getting big, slow down publisher */
    if (size > WEB_DDP_SERVER_SIZE_THRESHOLD) {
        cx_sleep(0, WEB_DDP_SERVER_BIG_LIST_SLEEP);
    }
/* $end */
}

/* ::corto::web::DDPServer::run() */
cx_void _web_DDPServer_run(web_DDPServer _this) {
/* $begin(::corto::web::DDPServer::run) */
    cx_event e;
    cx_ll events = cx_llNew();

    while (TRUE) {
        web_SockJsServer_poll(web_SockJsServer(_this), 0);
        if (web_SockJsServer(_this)->exiting) {
            break;
        }

        cx_lock(_this);
        while ((e = cx_llTakeFirst(_this->events))) {
            cx_llAppend(events, e);
        }
        cx_unlock(_this);
        while ((e = cx_llTakeFirst(events))) {
            cx_event_handle(e);
            cx_release(e);
            web_SockJsServer_poll(web_SockJsServer(_this), 1);
        }
    }
/* $end */
}
