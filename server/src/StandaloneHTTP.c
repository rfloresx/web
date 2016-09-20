/* $CORTO_GENERATED
 *
 * StandaloneHTTP.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */
#include "inttypes.h"
#include "math.h"
#include "mongoose.h"

/* Allocates memory and copies the content; free the memory manually when finished. */
static char* server_StandaloneHTTP_copyConnectionContent(struct mg_connection *conn) {
    char *content = corto_alloc(conn->content_len + 1);
    memcpy(content, conn->content, conn->content_len);
    content[conn->content_len] = '\0';
    return content;
}

static server_HTTP_Method server_StandaloneHTTP_methodNameToMethod(const char* methodString) {
    if (!strcmp(methodString, "GET")) {
        return Server_Get;
    } else if (!strcmp(methodString, "POST")) {
        return Server_Post;
    } else if (!strcmp(methodString, "PUT")) {
        return Server_Put;
    } else if (!strcmp(methodString, "DELETE")) {
        return Server_Delete;
    } else {
        return Server_None;
    }
}

static int server_StandaloneHTTP_handler(struct mg_connection *conn, enum mg_event ev) {
    int result = MG_TRUE;
    server_HTTP_Connection c = NULL;

    server_StandaloneHTTP this = server_StandaloneHTTP(conn->server_param);
    if (conn->connection_param) {
        c = server_HTTP_Connection(conn->connection_param);
    }

    switch (ev) {
    case MG_AUTH:
        break;
    case MG_WS_CONNECT:
        if (!conn->connection_param) {
            c = server_HTTP_ConnectionCreate(NULL, this);
            c->conn = (corto_word)conn;
            conn->connection_param = c;
        }
        server_HTTP_doOpen(this, c);
        break;
    case MG_CLOSE:
        if (c) {
            server_HTTP_doClose(this, c);
            corto_delete(c);
        }
        break;
    case MG_REQUEST:
        if (conn->is_websocket) {
            corto_string msg = server_StandaloneHTTP_copyConnectionContent(conn);
            server_HTTP_doMessage(this, c, msg);
            corto_dealloc(msg);
        } else {
            server_HTTP_Request r = {
                (corto_string)conn->uri,
                server_StandaloneHTTP_methodNameToMethod(conn->request_method),
                (corto_word)conn,
                FALSE
            };
            server_HTTP_doRequest(this, c, &r);
            result = r.file ? MG_MORE : MG_TRUE;
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


static void* server_StandaloneHTTP_threadRun(void *data) {
    server_StandaloneHTTP this = server_StandaloneHTTP(data);
    char port[6];
    sprintf(port, "%"PRIu16, server_HTTP(this)->port);

    /* Create mongoose server instance */
    struct mg_server *server = mg_create_server(
        this,
        server_StandaloneHTTP_handler);

    /* Store server instance on Corto object */
    this->server = (corto_word)server;

    mg_set_option(server, "listening_port", port);

    while (TRUE) {
        mg_poll_server(server, server_HTTP(this)->pollInterval);
        server_HTTP_doPoll(this);
        if (this->exiting) {
            break;
        }
    }

    mg_destroy_server(&server);
    return NULL;
}

/* $end */

corto_int16 _server_StandaloneHTTP_construct(
    server_StandaloneHTTP this)
{
/* $begin(corto/web/server/StandaloneHTTP/construct) */

    /* Register with port slot */
    if (!server_HTTP_set(server_HTTP(this)->port, this)) {
        corto_seterr("port %d already occupied by other server",
            server_HTTP(this)->port);
        goto error;
    }

    if (!server_HTTP(this)->pollInterval) {
        server_HTTP(this)->pollInterval = 200;
    }

    /* Start server thread */
    this->thread = (corto_word)corto_threadNew(
        server_StandaloneHTTP_threadRun,
        this);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _server_StandaloneHTTP_destruct(
    server_StandaloneHTTP this)
{
/* $begin(corto/web/server/StandaloneHTTP/destruct) */

    this->exiting = TRUE;
    if (this->thread) {
        corto_threadJoin((corto_thread)this->thread, NULL);
    }

    server_HTTP_destruct(this);

/* $end */
}

corto_void _server_StandaloneHTTP_write(
    server_StandaloneHTTP this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/StandaloneHTTP/write) */
    mg_websocket_printf((struct mg_connection *)c->conn, WEBSOCKET_OPCODE_TEXT, "%s", msg);

/* $end */
}
