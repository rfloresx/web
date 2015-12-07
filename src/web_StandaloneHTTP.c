/* $CORTO_GENERATED
 *
 * web_StandaloneHTTP.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "inttypes.h"
#include "math.h"
#include "mongoose.h"

/* Allocates memory and copies the content; free the memory manually when finished. */
static char* web_StandaloneHTTP_copyConnectionContent(struct mg_connection *conn) {
    char *content = corto_alloc(conn->content_len + 1);
    memcpy(content, conn->content, conn->content_len);
    content[conn->content_len] = '\0';
    return content;
}

static int web_StandaloneHTTP_handler(struct mg_connection *conn, enum mg_event ev) {
    int result = MG_TRUE;
    web_HTTP_Connection c;

    web_StandaloneHTTP this = web_StandaloneHTTP(conn->server_param);
    if (!conn->connection_param) {
        c = web_HTTP_ConnectionCreate(NULL, this);
        c->conn = (corto_word)conn;
        conn->connection_param = c;
    } else {
        c = web_HTTP_Connection(conn->connection_param);
    }

    switch (ev) {
    case MG_AUTH:
        break;
    case MG_WS_CONNECT:
        web_HTTP_doOpen(this, c);
        break;
    case MG_CLOSE:
        web_HTTP_doClose(this, c);
        corto_delete(c);
        break;
    case MG_REQUEST:
        if (conn->is_websocket) {
            corto_string msg = web_StandaloneHTTP_copyConnectionContent(conn);
            web_HTTP_doMessage(this, c, msg);
            corto_dealloc(msg);
        } else {
            web_HTTP_Request r = {
                (corto_string)conn->uri,
                (corto_word)conn
            };
            web_HTTP_doRequest(this, c, &r);
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


static void* web_StandaloneHTTP_threadRun(void *data) {
    web_StandaloneHTTP this = web_StandaloneHTTP(data);
    char port[6];
    sprintf(port, "%"PRIu16, web_HTTP(this)->port);

    /* Create mongoose server instance */
    struct mg_server *server = mg_create_server(
        this,
        web_StandaloneHTTP_handler);

    /* Store server instance on Corto object */
    this->server = (corto_word)server;

    mg_set_option(server, "listening_port", port);

    while (TRUE) {
        mg_poll_server(server, web_HTTP(this)->pollInterval);
        web_HTTP_doPoll(this);
        if (this->exiting) {
            break;
        }
    }

    mg_destroy_server(&server);
    return NULL;
}
/* $end */

corto_int16 _web_StandaloneHTTP_construct(web_StandaloneHTTP this) {
/* $begin(corto/web/StandaloneHTTP/construct) */

    /* Register with port slot */
    if (!web_HTTP_set(web_HTTP(this)->port, this)) {
        corto_seterr("port %d already occupied by other server",
            web_HTTP(this)->port);
        goto error;
    }

    if (!web_HTTP(this)->pollInterval) {
        web_HTTP(this)->pollInterval = 200;
    }

    /* Start server thread */
    this->thread = (corto_word)corto_threadNew(
        web_StandaloneHTTP_threadRun,
        this);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _web_StandaloneHTTP_destruct(web_StandaloneHTTP this) {
/* $begin(corto/web/StandaloneHTTP/destruct) */

    this->exiting = TRUE;
    if (this->thread) {
        corto_threadJoin((corto_thread)this->thread, NULL);
    }

    web_HTTP_destruct(this);

/* $end */
}

corto_void _web_StandaloneHTTP_write(web_StandaloneHTTP this, web_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/StandaloneHTTP/write) */

    mg_websocket_printf((struct mg_connection *)c->conn, WEBSOCKET_OPCODE_TEXT, msg);

/* $end */
}
