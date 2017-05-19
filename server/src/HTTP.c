/* $CORTO_GENERATED
 *
 * HTTP.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */
#define SERVER_MAX_SERVERS (64) /* Maximum number of services that may be active */

/*
 * Returns a static string representing the name of the HTTP method.
 */
static const char* _server_HTTP_getMethodName(server_HTTP_Method method)
{
    switch (method) {
    case Server_None:
        return "NONE";
    case Server_Get:
        return "GET";
    case Server_Head:
        return "HEAD";
    case Server_Post:
        return "POST";
    case Server_Put:
        return "PUT";
    case Server_Delete:
        return "DELETE";
    case Server_Trace:
        return "TRACE";
    case Server_Options:
        return "OPTIONS";
    case Server_Connect:
        return "CONNECT";
    case Server_Patch:
        return "PATCH";
    }
    return NULL;
}

static corto_mutex_s serverLock = CORTO_MUTEX_INITIALIZER;
static struct {
    server_HTTP server;
    corto_uint16 port;
} servers[SERVER_MAX_SERVERS];
/* $end */

void _server_HTTP_addService(
    server_HTTP this,
    server_Service s)
{
/* $begin(corto/web/server/HTTP/addService) */

    server_ServiceListAppend(this->services, s);
    corto_ok("HTTP: registered '%s' service", corto_fullpath(NULL, corto_typeof(s)));

/* $end */
}

void _server_HTTP_broadcast(
    server_HTTP this,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/broadcast) */
    corto_debug("HTTP: broadcast '%s'", msg);

    corto_iter it = corto_ll_iter(this->connections);
    while (corto_iter_hasNext(&it)) {
        server_HTTP c = corto_iter_next(&it);
        server_HTTP_write(this, c, msg);
    }

/* $end */
}

void _server_HTTP_destruct(
    server_HTTP this)
{
/* $begin(corto/web/server/HTTP/destruct) */

    server_HTTP_set(this->port, NULL);

/* $end */
}

void _server_HTTP_doClose(
    server_HTTP this,
    server_HTTP_Connection c)
{
/* $begin(corto/web/server/HTTP/doClose) */

    corto_iter it = corto_ll_iter(this->services);
    while (corto_iter_hasNext(&it)) {
        server_Service s = corto_iter_next(&it);
        server_Service_onClose(s, c);
    }

    server_HTTP_ConnectionListRemove(this->connections, c);

/* $end */
}

void _server_HTTP_doMessage(
    server_HTTP this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/doMessage) */

    corto_iter it = corto_ll_iter(this->services);
    while (corto_iter_hasNext(&it)) {
        server_Service s = corto_iter_next(&it);
        server_Service_onMessage(s, c, msg);
    }

/* $end */
}

void _server_HTTP_doOpen(
    server_HTTP this,
    server_HTTP_Connection c)
{
/* $begin(corto/web/server/HTTP/doOpen) */

    server_HTTP_ConnectionListAppend(this->connections, c);

    corto_iter it = corto_ll_iter(this->services);
    while (corto_iter_hasNext(&it)) {
        server_Service s = corto_iter_next(&it);
        server_Service_onOpen(s, c);
    }

/* $end */
}

void _server_HTTP_doPoll(
    server_HTTP this)
{
/* $begin(corto/web/server/HTTP/doPoll) */
    this->pollCount ++;
    if (this->pollCount == this->pollServiceRate) {
        corto_iter it = corto_ll_iter(this->services);
        while (corto_iter_hasNext(&it)) {
            server_Service s = corto_iter_next(&it);
            server_Service_onPoll(s);
        }
        this->pollCount = 0;
    }

/* $end */
}

void _server_HTTP_doRequest(
    server_HTTP this,
    server_HTTP_Connection c,
    server_HTTP_Request *r)
{
/* $begin(corto/web/server/HTTP/doRequest) */
    int handled = 0;

    server_HTTP_Request_setStatus(r, 200);

    corto_iter it = corto_ll_iter(this->services);
    while (corto_iter_hasNext(&it)) {
        server_Service s = corto_iter_next(&it);
        corto_string prefix = s->prefix ? s->prefix : "";

        int prefixLength = strlen(prefix);
        int uriLength = strlen(r->uri) - 1;
        if (!prefixLength || (!memcmp(r->uri + 1, prefix, prefixLength))) {
            corto_string uri = r->uri + (prefixLength ? (1 + prefixLength) : 0);
            if (prefixLength && (uriLength > prefixLength)) {
                uri += 1;
            }

            switch(r->method) {
            case Server_Get:
                handled = server_Service_onGet(s, c, r, uri);
                break;
            case Server_Post:
                handled = server_Service_onPost(s, c, r, uri);
                break;
            case Server_Put:
                handled = server_Service_onPut(s, c, r, uri);
                break;
            case Server_Delete:
                handled = server_Service_onDelete(s, c, r, uri);
                break;
            default:
                break;
            }

            /* Log if method-specific handlers were invoked */
            if (handled) {
                corto_ok(
                  "HTTP: %s: %s '%s'",
                  corto_idof(corto_typeof(s)),
                  _server_HTTP_getMethodName(r->method),
                  r->uri);
            }

            /* Log if generic handler was invoked */
            if (server_Service_onRequest(s, c, r, uri)) {
                corto_ok("HTTP: %s: %s '%s'",
                    corto_idof(corto_typeof(s)),
                    _server_HTTP_getMethodName(r->method),
                    r->uri);
                handled = TRUE;
            }

            if (handled) {
                break;
            }
        }
    }

    if (!handled) {
        corto_string str;
        corto_asprintf(&str, "Resource not found: %s '%s'", _server_HTTP_getMethodName(r->method), r->uri);
        server_HTTP_Request_setStatus(r, 404);
        server_HTTP_Request_reply(r, str);
        corto_dealloc(str);
        corto_warning("HTTP: %s '%s' not matched (404)", _server_HTTP_getMethodName(r->method), r->uri);
    }

/* $end */
}

server_HTTP _server_HTTP_get(
    uint16_t port)
{
/* $begin(corto/web/server/HTTP/get) */
    corto_int32 i = 0;

    corto_mutexLock(&serverLock);

    while ((i < SERVER_MAX_SERVERS) &&
           (servers[i].port != port))
    {
        i++;
    }

    corto_mutexUnlock(&serverLock);
    if (i >= SERVER_MAX_SERVERS) {
        return NULL;
    }
    return servers[i].server;
/* $end */
}

void _server_HTTP_removeService(
    server_HTTP this,
    server_Service s)
{
/* $begin(corto/web/server/HTTP/removeService) */

    server_ServiceListRemove(this->services, s);
    corto_ok("HTTP: removed %s service", corto_fullpath(NULL, corto_typeof(s)));

/* $end */
}

bool _server_HTTP_set(
    uint16_t port,
    server_HTTP server)
{
/* $begin(corto/web/server/HTTP/set) */
    corto_int32 i = 0;
    corto_bool result = TRUE;

    corto_mutexLock(&serverLock);

    while ((i < SERVER_MAX_SERVERS) &&
           (servers[i].port &&
           (servers[i].port != port)))
    {
        i++;
    }
    if (i >= SERVER_MAX_SERVERS) {
        result = FALSE;
    } else if (!servers[i].port || (servers[i].port == port)) {
        if (server && servers[i].server) {
            result = FALSE;
        } else if (server) {
            servers[i].server = server;
            servers[i].port = port;
        } else {
            servers[i].port = 0;
            servers[i].server = NULL;
        }
    }

    corto_mutexUnlock(&serverLock);

    corto_ok("HTTP: running server on port %d", port);

    return result;
/* $end */
}

void _server_HTTP_write_v(
    server_HTTP this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/write) */

    /* << Insert implementation >> */

/* $end */
}
