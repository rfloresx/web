/* $CORTO_GENERATED
 *
 * web_HTTP.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#define WEB_MAX_SERVERS (64) /* Maximum number of services that may be active */

static corto_mutex_s serverLock = CORTO_MUTEX_INITIALIZER;
static struct {
    web_HTTP server;
    corto_uint16 port;
} servers[WEB_MAX_SERVERS];
/* $end */

corto_void _web_HTTP_addService(web_HTTP this, web_Service s) {
/* $begin(corto/web/HTTP/addService) */

    web_ServiceListAppend(this->services, s);

/* $end */
}

corto_void _web_HTTP_destruct(web_HTTP this) {
/* $begin(corto/web/HTTP/destruct) */

    web_HTTP_set(this->port, NULL);

/* $end */
}

corto_void _web_HTTP_doClose(web_HTTP this, web_HTTP_Connection c) {
/* $begin(corto/web/HTTP/doClose) */

    web_ServiceListForeach(this->services, s) {
        web_Service_onClose(s, c);
    }

/* $end */
}

corto_void _web_HTTP_doMessage(web_HTTP this, web_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/HTTP/doMessage) */

    web_ServiceListForeach(this->services, s) {
        web_Service_onMessage(s, c, msg);
    }

/* $end */
}

corto_void _web_HTTP_doOpen(web_HTTP this, web_HTTP_Connection c) {
/* $begin(corto/web/HTTP/doOpen) */

    web_ServiceListForeach(this->services, s) {
        web_Service_onOpen(s, c);
    }

/* $end */
}

corto_void _web_HTTP_doRequest(web_HTTP this, web_HTTP_Connection c, web_HTTP_Request *r) {
/* $begin(corto/web/HTTP/doRequest) */

    web_ServiceListForeach(this->services, s) {
        web_Service_onRequest(s, c, r);
    }

/* $end */
}

web_HTTP _web_HTTP_get(corto_uint16 port) {
/* $begin(corto/web/HTTP/get) */
    corto_int32 i = 0;

    corto_mutexLock(&serverLock);

    while ((i < WEB_MAX_SERVERS) &&
           (!servers[i].port ||
           (servers[i].port == port)))
    {
        i++;
    }

    corto_mutexUnlock(&serverLock);

    return servers[i].server;
/* $end */
}

corto_void _web_HTTP_removeService(web_HTTP this, web_Service s) {
/* $begin(corto/web/HTTP/removeService) */

    web_ServiceListRemove(this->services, s);

/* $end */
}

corto_bool _web_HTTP_set(corto_uint16 port, web_HTTP server) {
/* $begin(corto/web/HTTP/set) */
    corto_int32 i = 0;
    corto_bool result = TRUE;

    corto_mutexLock(&serverLock);

    while ((i < WEB_MAX_SERVERS) &&
           (!servers[i].port ||
           (servers[i].port == port)))
    {
        i++;
    }

    if (servers[i].port == port) {
        if (server && servers[i].server) {
            result = FALSE;
        } else if (server) {
            servers[i].server = server;
        } else {
            servers[i].port = 0;
            servers[i].server = NULL;
        }
    }

    corto_mutexUnlock(&serverLock);

    return result;
/* $end */
}
