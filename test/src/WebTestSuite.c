/* $CORTO_GENERATED
 *
 * WebTestSuite.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "test.h"

corto_void _test_WebTestSuite_tc_matchEmptyRoute(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_matchEmptyRoute) */
    server_UrlMatch match = server_match("", "");
    test_assert(match != NULL);
    test_assert(match->matches == TRUE);
    // test_assert(match->params != NULL);
    corto_release(match);
/* $end */
}

corto_void _test_WebTestSuite_tc_matchRouteWithOneParam(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_matchRouteWithOneParam) */
    server_UrlMatch match = server_match("users/:userid", "users/12345");
    test_assert(match != NULL);
    test_assert(match->matches == TRUE);
    // test_assert(match->params != NULL);
    corto_release(match);
/* $end */
}

corto_void _test_WebTestSuite_tc_matchSimpleRoute(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_matchSimpleRoute) */
    server_UrlMatch match = server_match("users", "users");
    test_assert(match != NULL);
    test_assert(match->matches == TRUE);
    // test_assert(match->params != NULL);
    corto_release(match);
/* $end */
}
