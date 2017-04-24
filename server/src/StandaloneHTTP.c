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
#include "wshtp_server.h"

static char *server_StandaloneHTTP_copyConnectionContent(wshtp_conn_t *conn) {
    const char *message = wshtp_get_text(conn);
    if (message == NULL) {
        message = "";
    }
    char *content = corto_strdup(message);
    return content;
}

static int server_StandaloneHTTP_onOpen(wshtp_conn_t *conn, void *data){
    server_HTTP_Connection c = NULL;
    server_StandaloneHTTP this = server_StandaloneHTTP(data);

    void *connection_param = wshtp_conn_get_userdata(conn);
    if (connection_param) {
        c = server_HTTP_Connection(connection_param);
    } else {
        c = server_HTTP_ConnectionCreate(NULL, this);
        c->conn = (corto_word)conn;
        wshtp_conn_set_userdata(conn, c, NULL);
    }
    server_HTTP_doOpen(this, c);
    return EVHTP_RES_OK;
}

static int server_StandaloneHTTP_onMessage(wshtp_conn_t *conn, void *data){
    server_HTTP_Connection c = NULL;
    server_StandaloneHTTP this = server_StandaloneHTTP(data);

    void *connection_param = wshtp_conn_get_userdata(conn);
    if (connection_param) {
        c = server_HTTP_Connection(connection_param);
    }

    corto_string msg = server_StandaloneHTTP_copyConnectionContent(conn);
    server_HTTP_doMessage(this, c, msg);
    corto_dealloc(msg);
    return EVHTP_RES_OK;
}

static int server_StandaloneHTTP_onRequest(
  wshtp_conn_t *conn,
  server_HTTP_Method method,
  void *data)
{
    server_HTTP_Connection c = NULL;
    server_StandaloneHTTP this = server_StandaloneHTTP(data);

    void *connection_param = wshtp_conn_get_userdata(conn);
    if (connection_param) {
        c = server_HTTP_Connection(connection_param);
    }

    server_HTTP_Request r = {
        (char*)wshtp_request_get_path(conn),
        method,
        (corto_word)conn,
        false,
        NULL
    };
    
    int result;
    server_HTTP_doRequest(this, c, &r);
    result = EVHTP_RES_OK;
    if (r.garbage) {
        corto_iter it = corto_llIter(r.garbage);
        while (corto_iterHasNext(&it)) {
            corto_dealloc(corto_iterNext(&it));
        }
        corto_llFree(r.garbage);
    }
    return result;
}

static int server_StandaloneHTTP_onGet(wshtp_conn_t *conn, void *data) {
    return server_StandaloneHTTP_onRequest(conn, Server_Get, data);
}
static int server_StandaloneHTTP_onPost(wshtp_conn_t *conn, void *data) {
    return server_StandaloneHTTP_onRequest(conn, Server_Post, data);
}
static int server_StandaloneHTTP_onPut(wshtp_conn_t *conn, void *data) {
    return server_StandaloneHTTP_onRequest(conn, Server_Put, data);
}
static int server_StandaloneHTTP_onDelete(wshtp_conn_t *conn, void *data) {
    return server_StandaloneHTTP_onRequest(conn, Server_Delete, data);
}

static int server_StandaloneHTTP_onClose(wshtp_conn_t *conn, void *data){
    server_HTTP_Connection c = NULL;
    server_StandaloneHTTP this = server_StandaloneHTTP(data);

    void *connection_param = wshtp_conn_get_userdata(conn);
    if (connection_param) {
        c = server_HTTP_Connection(connection_param);
    }
    if (c) {
        server_HTTP_doClose(this, c);
        corto_delete(c);
    }
    return EVHTP_RES_OK;
}

static int server_StandaloneHTTP_onPoll(wshtp_conn_t *conn, void *data) {
    CORTO_UNUSED(conn);

    server_StandaloneHTTP this = server_StandaloneHTTP(data);
    server_HTTP_doPoll(this);
    return EVHTP_RES_OK;
}

static void* server_StandaloneHTTP_threadRun(void *data) {
    server_StandaloneHTTP this = server_StandaloneHTTP(data);

    wshtp_server_t *server = wshtp_server_new();
    this->server = (corto_word)server;

    evhtp_bind_socket(server->htp, "0.0.0.0", server_HTTP(this)->port, 1024);

    if (this->enable_ssl) {
        evhtp_ssl_cfg_t scfg = {
            .pemfile            = this->ssl_cert,
            .privfile           = this->ssl_pkey,
            .cafile             = NULL,
            .capath             = NULL,
            .ciphers            = "RC4+RSA:HIGH:+MEDIUM:+LOW",
            .ssl_opts           = SSL_OP_NO_SSLv2,
            .ssl_ctx_timeout    = 60 * 60 * 48,
            .verify_peer        = SSL_VERIFY_NONE,
            .verify_depth       = 42,
            .x509_verify_cb     = NULL,
            .x509_chk_issued_cb = NULL,
            .scache_type        = evhtp_ssl_scache_type_internal,
            .scache_size        = 1024,
            .scache_timeout     = 1024,
            .scache_init        = NULL,
            .scache_add         = NULL,
            .scache_get         = NULL,
            .scache_del         = NULL,
        };
        wshtp_ssl_init(server, &scfg);
    }

    wshtp_set_hook(server, WSHTP_ON_OPEN, server_StandaloneHTTP_onOpen, this);
    wshtp_set_hook(server, WSHTP_ON_MESSAGE, server_StandaloneHTTP_onMessage, this);
    wshtp_set_hook(server, WSHTP_ON_GET, server_StandaloneHTTP_onGet, this);
    wshtp_set_hook(server, WSHTP_ON_POST, server_StandaloneHTTP_onPost, this);
    wshtp_set_hook(server, WSHTP_ON_PUT, server_StandaloneHTTP_onPut, this);
    wshtp_set_hook(server, WSHTP_ON_DELETE, server_StandaloneHTTP_onDelete, this);
    wshtp_set_hook(server, WSHTP_ON_CLOSE, server_StandaloneHTTP_onClose, this);
    
    wshtp_set_pollInterval(server, server_HTTP(this)->pollInterval);
    wshtp_set_hook(server, WSHTP_ON_POLL, server_StandaloneHTTP_onPoll, this);

    wshtp_server_start(server);

    while (1) {
        printf("running!\n");
    }

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
    wshtp_send_text((wshtp_conn_t *)c->conn, msg);
/* $end */
}
