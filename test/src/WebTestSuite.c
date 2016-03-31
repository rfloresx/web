/* $CORTO_GENERATED
 *
 * WebTestSuite.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "test.h"

corto_void _test_WebTestSuite_tc_decodePercentageTwenty(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_decodePercentageTwenty) */
    corto_string s = server_urlDecode("hello%20world%20howdy");
    test_assert(s != NULL);
    test_assert(strcmp(s, "hello world howdy") == 0);
    corto_dealloc(s);
/* $end */
}

corto_void _test_WebTestSuite_tc_decodePlus(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_decodePlus) */
    corto_string s = server_urlDecode("hello+world+howdy");
    test_assert(s != NULL);
    test_assert(strcmp(s, "hello world howdy") == 0);
    corto_dealloc(s);
/* $end */
}

corto_void _test_WebTestSuite_tc_decodeSimple(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_decodeSimple) */
    corto_string s = server_urlDecode("hello");
    test_assert(s != NULL);
    test_assert(strcmp(s, "hello") == 0);
    corto_dealloc(s);
/* $end */
}

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

corto_void _test_WebTestSuite_tc_matchRouteWithTwoParts(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_matchRouteWithTwoParts) */
    server_UrlMatch match = server_match("api/users", "api/users");
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

corto_void _test_WebTestSuite_tc_queryToMapEmpty(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_queryToMapEmpty) */
    corto_rbtree map = server_queryToMap("");
    test_assert(map != NULL);
    test_assert(corto_rbtreeSize(map) == 0);
    corto_rbtreeFree(map);
/* $end */
}

corto_void _test_WebTestSuite_tc_queryToMapEncoded_1(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_queryToMapEncoded_1) */
    corto_rbtree map = server_queryToMap("userid=1&query=population+of+mexico+city&city=ciudad%20de%20mexico,%20mexico");
    test_assert(map != NULL);
    test_assert(corto_rbtreeSize(map) == 3);
    
    char* userid = corto_rbtreeGet(map, "userid");
    test_assert(userid != NULL);
    test_assert(strcmp(userid, "1") == 0);
    
    char* query = corto_rbtreeGet(map, "query");
    test_assert(query != NULL);
    test_assert(strcmp(query, "population of mexico city") == 0);

    char* city = corto_rbtreeGet(map, "city");
    test_assert(city != NULL);
    test_assert(strcmp(city, "ciudad de mexico, mexico") == 0);
    
    corto_rbtreeFree(map);
/* $end */
}

corto_void _test_WebTestSuite_tc_queryToMapEncoded_2(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_queryToMapEncoded_2) */
    corto_rbtree map = server_queryToMap("userid=1&query=population+of+mexico+city");
    test_assert(map != NULL);
    test_assert(corto_rbtreeSize(map) == 2);
    char* userid = corto_rbtreeGet(map, "userid");
    test_assert(userid != NULL);
    test_assert(strcmp(userid, "1") == 0);
    char* query = corto_rbtreeGet(map, "query");
    test_assert(query != NULL);
    test_assert(strcmp(query, "population of mexico city") == 0);
    corto_rbtreeFree(map);
/* $end */
}

corto_void _test_WebTestSuite_tc_queryToMapOneEntry(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_queryToMapOneEntry) */
    corto_rbtree map = server_queryToMap("userid=1");
    test_assert(map != NULL);
    test_assert(corto_rbtreeSize(map) == 1);
    char* value = corto_rbtreeGet(map, "userid");
    test_assert(value != NULL);
    test_assert(strcmp(value, "1") == 0);
    corto_rbtreeFree(map);
/* $end */
}

corto_void _test_WebTestSuite_tc_queryToMapTwoEntries(
    test_WebTestSuite this)
{
/* $begin(test/WebTestSuite/tc_queryToMapTwoEntries) */
    corto_rbtree map = server_queryToMap("userid=1&country=DE");
    test_assert(map != NULL);
    test_assert(corto_rbtreeSize(map) == 2);
    char* userid = corto_rbtreeGet(map, "userid");
    test_assert(userid != NULL);
    test_assert(strcmp(userid, "1") == 0);
    char* country = corto_rbtreeGet(map, "country");
    test_assert(country != NULL);
    test_assert(strcmp(country, "DE") == 0);
    corto_rbtreeFree(map);
/* $end */
}
