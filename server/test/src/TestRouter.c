/* $CORTO_GENERATED
 *
 * TestRouter.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <test.h>

/* $header() */

#include "MobyDickOrTheWhale.h"

/* $end */

corto_string _test_TestRouter_add(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/add) */
    corto_string aStr = server_HTTP_Request_getVar(request, "a");
    corto_string bStr = server_HTTP_Request_getVar(request, "b");
    int a = atoi(aStr);
    int b = atoi(bStr);
    corto_string buffer = NULL;
    corto_asprintf(&buffer, "result = %d", a + b);
    return buffer;
/* $end */
}

corto_string _test_TestRouter_hello(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/hello) */
    corto_string name = server_HTTP_Request_getVar(request, "name");
    corto_string buffer = NULL;
    corto_asprintf(&buffer, "Hello %s!", name);
    return buffer;
/* $end */
}

corto_string _test_TestRouter_index(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/index) */
    return corto_strdup("Congratulations, this is the endpoint /");
/* $end */
}

corto_string _test_TestRouter_mobyDick(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/mobyDick) */
    return corto_strdup(MOBY_DICK_OR_THE_WHALE);
/* $end */
}

corto_string _test_TestRouter_part1(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/part1) */
    return corto_strdup("Congratulations, this is the endpoint /part1");
/* $end */
}

corto_string _test_TestRouter_part2_part1(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/part2_part1) */
    return corto_strdup("Congratulations, this is the endpoint /part2/part1");
/* $end */
}

corto_string _test_TestRouter_part3_part1(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/part3_part1) */
    return corto_strdup("Congratulations, this is the endpoint /part3/part1");
/* $end */
}

corto_string _test_TestRouter_part3_part2(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/part3_part2) */
    return corto_strdup("Congratulations, this is the endpoint /part3/part2");
/* $end */
}

corto_string _test_TestRouter_post_users(
    test_TestRouter this,
    server_HTTP_Request *request)
{
/* $begin(test/TestRouter/post_users) */
    corto_string name = server_HTTP_Request_getVar(request, "name");
    corto_string email = server_HTTP_Request_getVar(request, "email");
    corto_string buffer = NULL;
    corto_asprintf(&buffer, "Created user %s with email %s", name, email);
    return buffer;
/* $end */
}
