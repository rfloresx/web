/* web_WebSocketServer.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */

#include "mongoose.h"

#define WEB_WEBSOCKETSERVER_DEFAULT_POLL_TIMEOUT 50

static void web_WebSocketServer_close(web_WebSocketServer _this, struct mg_connection *conn) {
    web_WebSocketConnection c = web_WebSocketConnection(conn->connection_param);
    if (_this->onClose._parent.procedure) {
        cx_call(_this->onClose._parent.procedure, NULL, _this->onClose._parent.instance, c);
    }
    c->conn = 0;
    cx_destruct(c);
    conn->connection_param = NULL;
}

static void web_WebSocketServer_open(web_WebSocketServer _this, struct mg_connection *conn) {
    cx_id id;
    sprintf(id, "C%d", _this->nextConnectionId);
    web_WebSocketConnection c = web_WebSocketConnection__declare(_this, id);
    c->conn = (cx_word)conn;
    if (web_WebSocketConnection__define(c)) {
        goto error;
    }
    conn->connection_param = c;
    if (_this->onOpen._parent.procedure) {
        cx_call(_this->onOpen._parent.procedure, NULL, _this->onOpen._parent.instance, c);
    }
error:;
}

static void web_WebSocketServer_message(web_WebSocketServer _this, struct mg_connection *conn) {
    web_WebSocketConnection c = web_WebSocketConnection(conn->connection_param);
    char *msg = cx_malloc(conn->content_len + 1);
    memcpy(msg, conn->content, conn->content_len);
    msg[conn->content_len] = '\0';
    if (_this->onMessage._parent.procedure) {
        cx_call(_this->onMessage._parent.procedure, NULL, _this->onMessage._parent.instance, c, msg);
    } else {
        puts("message handler is null!");
    }
}

static int web_WebSocketServer_handler(struct mg_connection *conn, enum mg_event ev) {
    if (ev != MG_POLL) {
        printf("handler!!!: %d\n", ev);
    }
    int result = MG_TRUE;
    web_WebSocketServer _this = web_WebSocketServer(conn->server_param);
    switch (ev) {
    case MG_AUTH:
        break;
    case MG_WS_CONNECT:
        web_WebSocketServer_open(_this, conn);
        break;
    case MG_CLOSE:
        web_WebSocketServer_close(_this, conn);
        break;
    case MG_REQUEST: 
        if (conn->is_websocket) {
            web_WebSocketServer_message(_this, conn);
        } else {
            result = MG_TRUE;
        }
        break;
    case MG_HTTP_ERROR:
        result = MG_FALSE;
        break;
    case MG_POLL:
        result = MG_FALSE;
        break;
    case MG_CONNECT:
    case MG_WS_HANDSHAKE:
    case MG_REPLY:
    case MG_RECV:
        result = MG_FALSE;
        break;
    }

    return result;
}

static void* web_WebSocketServer_threadRun(void *data) {
    web_WebSocketServer _this = web_WebSocketServer(data);
    struct mg_server *server = mg_create_server(_this, web_WebSocketServer_handler);
    mg_set_option(server, "listening_port", "8000");
    _this->server = (cx_word)server;
    web_WebSocketServer_run(_this);
    mg_destroy_server(&server);
    return NULL;
}

/* $end */

/* ::cortex::web::WebSocketServer::construct() */
cx_int16 web_WebSocketServer_construct(web_WebSocketServer _this) {
/* $begin(::cortex::web::WebSocketServer::construct) */
    if (_this->pollTimemoutMillis == 0) {
        _this->pollTimemoutMillis = WEB_WEBSOCKETSERVER_DEFAULT_POLL_TIMEOUT;
    }
    _this->thread = (cx_word)cx_threadNew(web_WebSocketServer_threadRun, _this);
    // TODO THIS IS AN UGLY HACK, WHY DOESNT IT WORK
    cx_object onMessage = cx_resolve(cx_typeof(_this), "onMessage");
    if (onMessage) {
        _this->onMessage._parent.procedure = onMessage;
        _this->onMessage._parent.instance = _this;
    }
    return 0;
/* $end */
}

/* ::cortex::web::WebSocketServer::destruct() */
cx_void web_WebSocketServer_destruct(web_WebSocketServer _this) {
/* $begin(::cortex::web::WebSocketServer::destruct) */
    _this->exiting = TRUE;
    cx_threadJoin((cx_thread)_this->thread, NULL);
/* $end */
}

/* ::cortex::web::WebSocketServer::poll() */
cx_void web_WebSocketServer_poll(web_WebSocketServer _this) {
/* $begin(::cortex::web::WebSocketServer::poll) */
    struct mg_server *server = (struct mg_server *)_this->server;
    mg_poll_server(server, _this->pollTimemoutMillis);
/* $end */
}

/* ::cortex::web::WebSocketServer::run() */
cx_void web_WebSocketServer_run_v(web_WebSocketServer _this) {
/* $begin(::cortex::web::WebSocketServer::run) */
    while (TRUE) {
        web_WebSocketServer_poll(_this);
        if (_this->exiting) {
            break;
        }
    }
/* $end */
}
