/* $CORTO_GENERATED
 *
 * web_Service.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

corto_int16 _web_Service_construct(web_Service this) {
/* $begin(corto/web/Service/construct) */
    web_HTTP server = NULL;

    /* Fetch HTTP server for port. If none is registered, automatically create
     * a standalone server */
    while (!server) {
        server = web_HTTP_get(web_Service(this)->port);
        if (!server) {
            server = web_HTTP(web_StandaloneHTTPCreate(
                web_Service(this)->port, 200));

            /* Server can be NULL if another thread registered a server for the
             * same port. Therefore, keep trying until a valid server object has
             * been obtained. */
        }
    }

    corto_setref(&this->server, server);

    web_HTTP_addService(server, this);

    return 0;
/* $end */
}

corto_void _web_Service_onClose_v(web_Service this, web_HTTP_Connection c) {
/* $begin(corto/web/Service/onClose) */

    /* << Insert implementation >> */

/* $end */
}

corto_void _web_Service_onMessage_v(web_Service this, web_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/Service/onMessage) */

    /* << Insert implementation >> */

/* $end */
}

corto_void _web_Service_onOpen_v(web_Service this, web_HTTP_Connection c) {
/* $begin(corto/web/Service/onOpen) */

    /* << Insert implementation >> */

/* $end */
}

corto_void _web_Service_onRequest_v(web_Service this, web_HTTP_Connection c, web_HTTP_Request *r) {
/* $begin(corto/web/Service/onRequest) */

    /* << Insert implementation >> */

/* $end */
}
