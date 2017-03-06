/* $CORTO_GENERATED
 *
 * RouterTest.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <include/test.h>

/* $header() */

#include "MobyDickOrTheWhale.h"

/* $end */

corto_void _test_RouterTest_tc_GetEndpointUnknown(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetEndpointUnknown) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/endpoint-that-doesnt-exist", "");
    test_assertint(result.status, 404);
    test_assertstr(result.response, "Resource not found: GET /endpoint-that-doesnt-exist");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetIndex(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetIndex) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "Congratulations, this is the endpoint /");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetMessage(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetMessage) */
    char* message = "Congratulations this is a messsage, an instance member, to be precise";
    test_TestRouter router = test_TestRouterCreate(3000, message);
    web_client_Result result = web_client_get("localhost:3000/message", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, message);
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetMessageWithUriParam(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetMessageWithUriParam) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/add/sugar", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "Added sugar!");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetRouteWithCommonParts(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetRouteWithCommonParts) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result1 = web_client_get("localhost:3000/part3/part1", "");
    test_assertint(result1.status, 200);
    test_assertstr(result1.response, "Congratulations, this is the endpoint /part3/part1");
    web_client_Result result2 = web_client_get("localhost:3000/part3/part2", "");
    test_assertint(result2.status, 200);
    test_assertstr(result2.response, "Congratulations, this is the endpoint /part3/part2");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetRouteWithLargeBody(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetRouteWithLargeBody) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/moby-dick", "");
    test_assertint(result.status, 200);
    test_assertint(strlen(result.response), 3554);
    test_assertstr(result.response, (corto_string)MOBY_DICK_OR_THE_WHALE);
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetRouteWithMultipleParams(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetRouteWithMultipleParams) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/add?a=11&b=7", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "result = 18");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetRouteWithOnePart(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetRouteWithOnePart) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/part1", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "Congratulations, this is the endpoint /part1");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetRouteWithParams(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetRouteWithParams) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/hello?name=Nietzsche", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "Hello Nietzsche!");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetRouteWithPercentEncodingParams(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetRouteWithPercentEncodingParams) */
    /*
     * https://tools.ietf.org/html/rfc3986#section-2.1
     */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/hello?name=Herr%20Schopenhauer", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "Hello Herr Schopenhauer!");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_GetRouteWithTwoParts(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_GetRouteWithTwoParts) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_get("localhost:3000/part2/part1", "");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "Congratulations, this is the endpoint /part2/part1");
    corto_delete(router);
/* $end */
}

corto_void _test_RouterTest_tc_PostEndpoint(
    test_RouterTest this)
{
/* $begin(test/RouterTest/tc_PostEndpoint) */
    test_TestRouter router = test_TestRouterCreate(3000, "");
    web_client_Result result = web_client_post("localhost:3000/users", "name=Ada&email=ada@lovelace.com");
    test_assertint(result.status, 200);
    test_assertstr(result.response, "Created user Ada with email ada@lovelace.com");
    corto_delete(router);
/* $end */
}
