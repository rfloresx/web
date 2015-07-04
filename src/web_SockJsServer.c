/* web_SockJsServer.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */
#include "parson.h"
#include "mongoose.h"

#define WEB_SOCKJSSERVER_DEFAULT_POLL_TIMEOUT       (50)
#define WEB_SOCKJSSERVER_DEFAULT_HEARTBEAT_TIMEOUT  (25)

static void web_SockJsServer_close(web_SockJsServer _this, struct mg_connection *conn) {
    web_SockJsServer_Connection c = web_SockJsServer_Connection(conn->connection_param);
    if (c) {
        if (_this->onClose._parent.procedure) {
            cx_call(_this->onClose._parent.procedure, NULL, _this->onClose._parent.instance, c);
        }
        c->conn = 0;
        cx_destruct(c);
        conn->connection_param = NULL;
    }
}

static void web_SockJsServer_open(web_SockJsServer _this, struct mg_connection *conn) {
    cx_string id = web_random(17);
    web_SockJsServer_Connection c = web_SockJsServer_Connection__declare(_this, id);
    cx_dealloc(id);
    c->conn = (cx_word)conn;
    if (web_SockJsServer_Connection__define(c, NULL)) {
        goto error;
    }
    conn->connection_param = c;
    if (_this->onOpen._parent.procedure) {
        cx_call(_this->onOpen._parent.procedure, NULL, _this->onOpen._parent.instance, c);
    }
error:;
}

static void web_SockJsServer_message(web_SockJsServer _this, struct mg_connection *conn) {
    web_SockJsServer_Connection c = web_SockJsServer_Connection(conn->connection_param);
    if (_this->onMessage._parent.procedure) {
        if (conn->content_len) {
            char *msg = cx_malloc(conn->content_len + 1);
            memcpy(msg, conn->content, conn->content_len);
            msg[conn->content_len] = '\0';

            /* Parse & unpack message */
            JSON_Value *root = json_parse_string(msg);
            if (!root) {
                goto error;
            }

            if (json_value_get_type(root) == JSONArray) {
                JSON_Array *messages = json_value_get_array(root);
                cx_uint32 i;
                for (i = 0; i < json_array_get_count(messages); i++) {
                    const char *message = json_array_get_string(messages, i);
                    cx_call(_this->onMessage._parent.procedure, NULL, _this->onMessage._parent.instance, c, message);
                }
            }

            json_value_free(root);
        }
    }
error:;
}

static int web_SockJsServer_request(struct mg_connection *conn, enum mg_event ev) {
    int result = MG_TRUE;
    
    web_SockJsServer _this = web_SockJsServer(conn->server_param);
    switch (ev) {
    case MG_AUTH:
        break;
    case MG_WS_CONNECT:
        /* Send open message fast since SockJS uses this to determine latency */
        mg_websocket_printf(conn, WEBSOCKET_OPCODE_TEXT, "o");
        web_SockJsServer_open(_this, conn);
        break;
    case MG_CLOSE:
        web_SockJsServer_close(_this, conn);
        break;
    case MG_REQUEST:
        if (conn->is_websocket) {
            web_SockJsServer_message(_this, conn);
        } else {
            if (!strcmp(conn->uri, "/sockjs/info")) {
                mg_send_header(conn, "Access-Control-Allow-Origin", "*");
                mg_printf_data(
                    conn, 
                    "{\"websocket\":true,\"origins\":[\"*:*\"],\"cookie_needed\":false,\"entropy\":%u}", 
                    10000000000 * rand());
            }
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

static void* web_SockJsServer_threadRun(void *data) {
    web_SockJsServer _this = web_SockJsServer(data);
    struct mg_server *server = mg_create_server(_this, web_SockJsServer_request);
    mg_set_option(server, "listening_port", "8000");
    _this->server = (cx_word)server;
    web_SockJsServer_run(_this);
    mg_destroy_server(&server);
    return NULL;
}
/* $end */

/* ::cortex::web::SockJsServer::construct() */
cx_int16 web_SockJsServer_construct(web_SockJsServer _this) {
/* $begin(::cortex::web::SockJsServer::construct) */
    if (_this->pollTimemoutMillis == 0) {
        _this->pollTimemoutMillis = WEB_SOCKJSSERVER_DEFAULT_POLL_TIMEOUT;
    }
    _this->thread = (cx_word)cx_threadNew(web_SockJsServer_threadRun, _this);
    return 0;
/* $end */
}

/* ::cortex::web::SockJsServer::destruct() */
cx_void web_SockJsServer_destruct(web_SockJsServer _this) {
/* $begin(::cortex::web::SockJsServer::destruct) */
    _this->exiting = TRUE;
    cx_threadJoin((cx_thread)_this->thread, NULL);
/* $end */
}

/* ::cortex::web::SockJsServer::poll() */
cx_void web_SockJsServer_poll(web_SockJsServer _this) {
/* $begin(::cortex::web::SockJsServer::poll) */
    struct mg_server *server = (struct mg_server *)_this->server;
    mg_poll_server(server, _this->pollTimemoutMillis);
    _this->timeElapsed += _this->pollTimemoutMillis;

    /* Send heartbeats for all live connections every n seconds */
    if (_this->timeElapsed >= (WEB_SOCKJSSERVER_DEFAULT_HEARTBEAT_TIMEOUT * 1000)) {
        cx_ll scope = cx_scopeClaim(_this);
        cx_iter iter = cx_llIter(scope);
        while (cx_iterHasNext(&iter)) {
            cx_object o = cx_iterNext(&iter);
            if (cx_instanceof(cx_type(web_SockJsServer_Connection_o), o)) {
                web_SockJsServer_Connection c = web_SockJsServer_Connection(o);
                mg_websocket_printf((struct mg_connection *)c->conn, WEBSOCKET_OPCODE_TEXT, "h");
            }
        }
        cx_scopeRelease(scope);
        _this->timeElapsed = 0;
    }
/* $end */
}

/* ::cortex::web::SockJsServer::run() */
cx_void web_SockJsServer_run_v(web_SockJsServer _this) {
/* $begin(::cortex::web::SockJsServer::run) */
    while (TRUE) {
        web_SockJsServer_poll(_this);
        if (_this->exiting) {
            break;
        }
    }
/* $end */
}
