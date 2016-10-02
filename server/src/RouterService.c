/* $CORTO_GENERATED
 *
 * RouterService.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

corto_int16 _server_RouterService_onRequest(
    server_RouterService this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/RouterService/onRequest) */
    corto_any result = {corto_type(corto_string_o), &result, FALSE};
    corto_any param = {corto_type(server_HTTP_Request_o), r, FALSE};

    if (corto_router_match(corto_typeof(this), uri, param, result)) {
        goto nomatch;
    } else {
        server_HTTP_Request_reply(r, *(corto_string*)result.value);
        corto_dealloc(*(corto_string*)result.value);
    }

    return 1;
nomatch:
    /* Let HTTP server know Service doesn't handle request */
    return 0;
/* $end */
}
