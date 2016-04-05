/* $CORTO_GENERATED
 *
 * RouteService.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

corto_int16 _server_RouteService_construct(
    server_RouteService this)
{
/* $begin(corto/web/server/RouteService/construct) */
    return server_Service_construct(server_Service(this));
/* $end */
}

corto_int16 _server_RouteService_onRequest(
    server_RouteService this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/RouteService/onRequest) */
    corto_objectseq routesseq = corto_scopeClaim(corto_typeof(this));
    corto_objectseqForeach(routesseq, _route) {
        if (!corto_instanceof(server_Route_o, _route)) {
            continue;
        }
        server_Route route = _route;
        corto_string pattern = route->pattern;
        if (pattern == NULL || route->method == Server_None) {
            corto_error("route %s does not have a pattern or method", corto_idof(route));
            continue;
        }
        if (r->method != route->method) {
            continue;
        }
        server_UrlMatch match = server_match(pattern, uri);
        if (match == NULL) {
            corto_error("error matching pattern and URL: %s", pattern, uri);
            corto_seterr(NULL);
            continue;
        }
        corto_bool matched = match->matches;
        if (matched) {
            /* TODO use the match dictionary */
            void* result = NULL;
            corto_call(corto_function(route), &result, this, r);
        }
        corto_release(match);
        if (matched) {
            return 1;
        }
    }
    return 0;
/* $end */
}
