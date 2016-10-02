/* $CORTO_GENERATED
 *
 * RouterService.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */
/* TODO this was shamefully copied from HTTP.c */

static const char* _RouterService_getMethodName(server_HTTP_Method method)
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

/* $end */

corto_int16 _server_RouterService_construct(
    server_RouterService this)
{
/* $begin(corto/web/server/RouterService/construct) */
    return server_Service_construct(this);
/* $end */
}

corto_int16 _server_RouterService_onRequest(
    server_RouterService this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/RouterService/onRequest) */
    corto_string resultStr = NULL;
    corto_any result = {corto_type(corto_string_o), &resultStr, FALSE};
    corto_any param = {corto_type(server_HTTP_Request_o), r, FALSE};

    if (corto_router_match(corto_typeof(this), uri, param, result)) {
        goto nomatch;
    } else {
        if (resultStr) {
            server_HTTP_Request_reply(r, resultStr);
            corto_dealloc(resultStr);
        } else {
            server_HTTP_Request_reply(r, "");
        }
    }

    return 1;
nomatch:
    server_HTTP_Request_setStatus(r, 404);
    const char* methodName = _RouterService_getMethodName(r->method);
    char* message = NULL;
    corto_asprintf(&message, "Cannot find endpoint %s %s", methodName, r->uri);
    message = message ? message : "Cannot find endpoint";
    server_HTTP_Request_reply(r, message);
    return 0;
/* $end */
}
