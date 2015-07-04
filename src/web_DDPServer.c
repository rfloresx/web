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

#include "mongoose.h"
#include "parson.h"

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
        if (!session || !(ddpSession = cx_resolve(_this, (cx_string)session))) {
            char *sessionId = web_random(17);
            ddpSession = web_DDPServer_Session__declare(_this, sessionId);
            web_DDPServer_Session__define(ddpSession, NULL, conn);
            cx_set(&conn->data, ddpSession);
            cx_dealloc(sessionId);
        } else {
            cx_set(&ddpSession->conn, conn);
            cx_set(&conn->data, ddpSession);
            cx_free(ddpSession);
        }

        web_DDPServer_Session_connected(ddpSession);
    }

    return;
failed:;
    web_DDPServer_Session_failed(conn);
}

cx_void web_DDPServer_ping(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Object *json) {
    CX_UNUSED(_this);
    const char *id = json_object_get_string(json, "id");
    web_DDPServer_Session_pong(conn->data, (cx_string)id);
}

/* $end */

/* ::cortex::web::DDPServer::construct() */
cx_int16 web_DDPServer_construct(web_DDPServer _this) {
/* $begin(::cortex::web::DDPServer::construct) */

    /* Set the handlers of the SockJsServer base */
    cx_set(&web_SockJsServer(_this)->onClose._parent.procedure, web_DDPServer_onClose_o);
    cx_set(&web_SockJsServer(_this)->onClose._parent.instance, _this);

    cx_set(&web_SockJsServer(_this)->onMessage._parent.procedure, web_DDPServer_onMessage_o);
    cx_set(&web_SockJsServer(_this)->onMessage._parent.instance, _this);

    return web_SockJsServer_construct(web_SockJsServer(_this));
/* $end */
}

/* ::cortex::web::DDPServer::onClose(::cortex::web::SockJsServer::Connection conn) */
cx_void web_DDPServer_onClose(web_DDPServer _this, web_SockJsServer_Connection conn) {
/* $begin(::cortex::web::DDPServer::onClose) */
    CX_UNUSED(_this);
    cx_destruct(conn->data);
/* $end */
}

/* ::cortex::web::DDPServer::onMessage(::cortex::web::SockJsServer::Connection conn,string message) */
cx_void web_DDPServer_onMessage(web_DDPServer _this, web_SockJsServer_Connection conn, cx_string message) {
/* $begin(::cortex::web::DDPServer::onMessage) */
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

/* ::cortex::web::DDPServer::post(event e) */
/* $header(::cortex::web::DDPServer::post) */
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
cx_void web_DDPServer_post(web_DDPServer _this, cx_event e) {
/* $begin(::cortex::web::DDPServer::post) */
    cx_uint32 size = 0;
    cx_observableEvent e2;

    cx_lock(_this);
    /* Check if there is already another event in the queue for the same object.
     * if so, replace event with latest update. */
    if ((e2 = web_DDPServer_findRelatedEvent(_this, cx_observableEvent(e)))) {
        cx_llReplace(_this->events, e2, e);
        cx_free(e2);
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

/* ::cortex::web::DDPServer::step() */
cx_bool web_DDPServer_step(web_DDPServer _this) {
/* $begin(::cortex::web::DDPServer::step) */
    cx_event e;
    cx_ll events = cx_llNew();
    cx_bool result = TRUE;
    web_WebSocketServer __this = web_WebSocketServer(_this);

    web_WebSocketServer_poll(__this);
    if (__this->exiting) {
        result = FALSE;
    }
    
    struct mg_server *server = (struct mg_server *)__this->server;
    cx_lock(_this);
    while ((e = cx_llTakeFirst(_this->events))) {
        cx_llAppend(events, e);
    }
    cx_unlock(_this);
    while ((e = cx_llTakeFirst(events))) {
        cx_event_handle(e);
        cx_free(e);
        mg_poll_server(server, 1);
    }
    
    return result;
/* $end */
}
