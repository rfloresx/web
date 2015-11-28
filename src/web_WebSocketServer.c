/* $CORTO_GENERATED
 *
 * web_WebSocketServer.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */

#include "inttypes.h"
#include "math.h"
#include "mongoose.h"


#define WEB_WEBSOCKETSERVER_DEFAULT_POLL_TIMEOUT 50

/* Allocates memory and copies the content; free the memory manually when finished. */
static char* web_WebSocketServer_copyConnectionContent(struct mg_connection *conn) {
    char *content = corto_alloc(conn->content_len + 1);
    memcpy(content, conn->content, conn->content_len);
    content[conn->content_len] = '\0';
    return content;
}

static void web_WebSocketServer_close(web_WebSocketServer this, struct mg_connection *conn) {
    web_WebSocketConnection c = web_WebSocketConnection(conn->connection_param);
    if (this->onClose._parent.procedure) {
        corto_call(this->onClose._parent.procedure, NULL, this->onClose._parent.instance, c);
    }
    c->conn = 0;
    conn->connection_param = NULL;
    corto_delete(c);
}

static void web_WebSocketServer_open(web_WebSocketServer this, struct mg_connection *conn) {
    char *name = web_random(17);
    web_WebSocketConnection c = web_WebSocketConnectionDeclareChild(this, name);
    corto_dealloc(name);
    c->conn = (corto_word)conn;
    if (web_WebSocketConnectionDefine(c, NULL)) {
        goto error;
    }

    conn->connection_param = c;

    if (this->onOpen._parent.procedure) {
        web_WebSocketConnection c = web_WebSocketConnection(conn->connection_param);
        corto_call(this->onOpen._parent.procedure, NULL, this->onOpen._parent.instance, c);
    }
error:;
}

static void web_WebSocketServer_message(web_WebSocketServer this, struct mg_connection *conn) {
    web_WebSocketConnection c = web_WebSocketConnection(conn->connection_param);
    char *msg = web_WebSocketServer_copyConnectionContent(conn);
    if (this->onMessage._parent.procedure) {
        corto_call(this->onMessage._parent.procedure, NULL, this->onMessage._parent.instance, c, msg);
    }
    corto_dealloc(msg);
}

static int web_WebSocketServer_handler(struct mg_connection *conn, enum mg_event ev) {
    int result = MG_TRUE;

    web_WebSocketServer this = web_WebSocketServer(conn->server_param);
    switch (ev) {
    case MG_AUTH:
        break;
    case MG_WS_CONNECT:
        web_WebSocketServer_open(this, conn);
        break;
    case MG_CLOSE:
        if (conn->is_websocket) {
            web_WebSocketServer_close(this, conn);
        }
        break;
    case MG_REQUEST:
        if (conn->is_websocket) {
            web_WebSocketServer_message(this, conn);
        } else {
            corto_error("WebSocketServer received Http message");
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
    web_WebSocketServer this = web_WebSocketServer(data);
    char port[6];
    sprintf(port, "%"PRIu16, this->port);
    struct mg_server *server = mg_create_server(this, web_WebSocketServer_handler);
    mg_set_option(server, "listening_port", port);
    this->server = (corto_word)server;
    web_WebSocketServer_run(this);
    mg_destroy_server(&server);
    return NULL;
}

/* $end */

corto_int16 _web_WebSocketServer_construct(web_WebSocketServer this) {
/* $begin(corto/web/WebSocketServer/construct) */
    if (this->pollTimemoutMillis == 0) {
        this->pollTimemoutMillis = WEB_WEBSOCKETSERVER_DEFAULT_POLL_TIMEOUT;
    }
    this->thread = (corto_word)corto_threadNew(web_WebSocketServer_threadRun, this);
    // TODO THIS IS AN UGLY HACK, WHY DOESNT IT WORK
    corto_object onMessage = corto_resolve(corto_typeof(this), "onMessage");
    if (onMessage) {
        this->onMessage._parent.procedure = onMessage;
        this->onMessage._parent.instance = this;
    }
    return 0;
/* $end */
}

corto_void _web_WebSocketServer_destruct(web_WebSocketServer this) {
/* $begin(corto/web/WebSocketServer/destruct) */
    this->exiting = TRUE;
    corto_threadJoin((corto_thread)this->thread, NULL);
/* $end */
}

corto_void _web_WebSocketServer_poll(web_WebSocketServer this) {
/* $begin(corto/web/WebSocketServer/poll) */
    struct mg_server *server = (struct mg_server *)this->server;
    mg_poll_server(server, this->pollTimemoutMillis);
/* $end */
}

corto_void _web_WebSocketServer_run_v(web_WebSocketServer this) {
/* $begin(corto/web/WebSocketServer/run) */
    while (TRUE) {
        web_WebSocketServer_poll(this);
        if (this->exiting) {
            break;
        }
    }
/* $end */
}
