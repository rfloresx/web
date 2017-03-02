/* $CORTO_GENERATED
 *
 * Router.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

corto_int32 _server_Router_matchRoute(
    server_Router this,
    corto_route route,
    corto_stringseq pattern,
    corto_any param,
    corto_any routerData)
{
/* $begin(corto/web/server/Router/matchRoute) */
    corto_assert(
      corto_instanceofType(server_HTTP_Request_o, param.type),
      "web/server/Router parameter is not HTTP/Request");

    corto_int32 result = -1;
    server_HTTP_Request *r = param.value;
    corto_bool match = FALSE;

    if ((r->method == Server_Get) && (corto_instanceof(server_GET_o, route))) {
        match = TRUE;
    } else if ((r->method == Server_Post) && (corto_instanceof(server_POST_o, route))) {
        match = TRUE;
    } else if ((r->method == Server_Put) && (corto_instanceof(server_PUT_o, route))) {
        match = TRUE;
    } else if ((r->method == Server_Delete) && (corto_instanceof(server_DELETE_o, route))) {
        match = TRUE;
    }

    if (match) {
        result = corto_routerimpl_matchRoute_v(this, route, pattern, param, NULL);
    }

    return result;
/* $end */
}
