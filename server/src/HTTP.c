/* $CORTO_GENERATED
 *
 * HTTP.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */
#define SERVER_MAX_SERVERS (64) /* Maximum number of services that may be active */

static corto_mutex_s serverLock = CORTO_MUTEX_INITIALIZER;
static struct {
    server_HTTP server;
    corto_uint16 port;
} servers[SERVER_MAX_SERVERS];
/* $end */

corto_void _server_HTTP_addService(server_HTTP this, server_Service s) {
/* $begin(corto/web/server/HTTP/addService) */

    server_ServiceListAppend(this->services, s);

/* $end */
}

corto_void _server_HTTP_broadcast(server_HTTP this, corto_string msg) {
/* $begin(corto/web/server/HTTP/broadcast) */

    server_HTTP_ConnectionListForeach(this->connections, c) {
        server_HTTP_write(this, c, msg);
    }

/* $end */
}

corto_void _server_HTTP_destruct(server_HTTP this) {
/* $begin(corto/web/server/HTTP/destruct) */

    server_HTTP_set(this->port, NULL);

/* $end */
}

corto_void _server_HTTP_doClose(server_HTTP this, server_HTTP_Connection c) {
/* $begin(corto/web/server/HTTP/doClose) */

    server_ServiceListForeach(this->services, s) {
        server_Service_onClose(s, c);
    }

    server_HTTP_ConnectionListRemove(this->connections, c);

/* $end */
}

corto_void _server_HTTP_doMessage(server_HTTP this, server_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/server/HTTP/doMessage) */

    server_ServiceListForeach(this->services, s) {
        server_Service_onMessage(s, c, msg);
    }

/* $end */
}

corto_void _server_HTTP_doOpen(server_HTTP this, server_HTTP_Connection c) {
/* $begin(corto/web/server/HTTP/doOpen) */

    server_HTTP_ConnectionListAppend(this->connections, c);

    server_ServiceListForeach(this->services, s) {
        server_Service_onOpen(s, c);
    }

/* $end */
}

corto_void _server_HTTP_doPoll(server_HTTP this) {
/* $begin(corto/web/server/HTTP/doPoll) */

    server_ServiceListForeach(this->services, s) {
        server_Service_onPoll(s);
    }

/* $end */
}

corto_void _server_HTTP_doRequest(server_HTTP this, server_HTTP_Connection c, server_HTTP_Request *r) {
/* $begin(corto/web/server/HTTP/doRequest) */

    server_ServiceListForeach(this->services, s) {
        int prefixLength = strlen(s->prefix);
        int uriLength = strlen(r->uri) - 1;
        if (!prefixLength || (!memcmp(r->uri + 1, s->prefix, prefixLength))) {
            corto_string uri = r->uri + 1 + prefixLength;
            if (uriLength > prefixLength) {
                uri += 1;
            }
            server_Service_onRequest(s, c, r, uri);
        }
    }

/* $end */
}

server_HTTP _server_HTTP_get(corto_uint16 port) {
/* $begin(corto/web/server/HTTP/get) */
    corto_int32 i = 0;

    corto_mutexLock(&serverLock);

    while ((i < SERVER_MAX_SERVERS) &&
           (servers[i].port != port))
    {
        i++;
    }

    corto_mutexUnlock(&serverLock);

    return servers[i].server;
/* $end */
}

corto_void _server_HTTP_removeService(server_HTTP this, server_Service s) {
/* $begin(corto/web/server/HTTP/removeService) */

    server_ServiceListRemove(this->services, s);

/* $end */
}

corto_bool _server_HTTP_set(corto_uint16 port, server_HTTP server) {
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

    if (!servers[i].port || (servers[i].port == port)) {
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

    return result;
/* $end */
}

corto_void _server_HTTP_write_v(server_HTTP this, server_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/server/HTTP/write) */

    /* << Insert implementation >> */

/* $end */
}
