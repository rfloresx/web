/* $CORTO_GENERATED
 *
 * Service.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

corto_int16 _server_Service_construct(
    server_Service this)
{
/* $begin(corto/web/server/Service/construct) */
    server_HTTP server = NULL;

    if (!this->prefix) {
        corto_setstr(&this->prefix, "");
    }

    /* Fetch HTTP server for port. If none is registered, automatically create
     * a standalone server */
    while (!server) {
        server = server_HTTP_get(server_Service(this)->port);
        if (!server) {
            server = server_HTTP(server_StandaloneHTTPCreate(
                server_Service(this)->port, 200));

            /* Server can be NULL if another thread registered a server for the
             * same port. Therefore, keep trying until a valid server object has
             * been obtained. */
        }
    }

    corto_setref(&this->server, server);

    server_HTTP_addService(server, this);

    return 0;
/* $end */
}

corto_void _server_Service_onClose_v(
    server_Service this,
    server_HTTP_Connection c)
{
/* $begin(corto/web/server/Service/onClose) */

/* $end */
}

corto_int16 _server_Service_onDelete_v(
    server_Service this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/Service/onDelete) */
    return 0;
/* $end */
}

corto_int16 _server_Service_onGet_v(
    server_Service this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/Service/onGet) */
    return 0;
/* $end */
}

corto_void _server_Service_onMessage_v(
    server_Service this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/Service/onMessage) */

    /* virtual method */

/* $end */
}

corto_void _server_Service_onOpen_v(
    server_Service this,
    server_HTTP_Connection c)
{
/* $begin(corto/web/server/Service/onOpen) */

    /* virtual method */

/* $end */
}

corto_void _server_Service_onPoll_v(
    server_Service this)
{
/* $begin(corto/web/server/Service/onPoll) */

    /* virtual method */

/* $end */
}

corto_int16 _server_Service_onPost_v(
    server_Service this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/Service/onPost) */
    return 0;
/* $end */
}

corto_int16 _server_Service_onPut_v(
    server_Service this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/Service/onPut) */
    return 0;
/* $end */
}

corto_int16 _server_Service_onRequest_v(
    server_Service this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/Service/onRequest) */

    return 0;

/* $end */
}
