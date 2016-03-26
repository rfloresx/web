/* $CORTO_GENERATED
 *
 * server.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */

static corto_ll server_getUrlTokens(corto_string s)
{
    corto_string buffer = corto_strdup(s);
    if (buffer == NULL) {
        goto errorStrdupBuffer;
    }
    corto_ll ll = corto_llNew();
    char* delimiters = "/";
    char* token = strtok(buffer, delimiters);
    while (token) {
        char* s = corto_strdup(token);
        if (s == NULL) {
            goto errorStrdupToken;
        }
        corto_llAppend(ll, s);
        token = strtok(NULL, delimiters);
    }
    corto_dealloc(buffer);
    return ll;
errorStrdupToken:
    corto_llFree(ll);
    corto_dealloc(buffer);
errorStrdupBuffer:
    return NULL;
}

/* $end */

server_UrlMatch _server_match(
    corto_string pattern,
    corto_string url)
{
/* $begin(corto/web/server/match) */
    corto_ll patternTokens = server_getUrlTokens(pattern);
    if (patternTokens == NULL) {
        goto errorPatternTokens;
    }
    corto_ll urlTokens = server_getUrlTokens(url);
    if (urlTokens == NULL) {
        goto errorUrlTokens;
    }
    corto_rbtree map = corto_rbtreeNew(corto_type(corto_string_o));
    if (map == NULL) {
        goto errorNewMap;
    }
    corto_bool matches = TRUE;
    corto_iter patternIter = corto_llIter(patternTokens);
    corto_iter urlIter = corto_llIter(urlTokens);
    corto_bool patternIterHasNext = corto_iterHasNext(&patternIter) ? TRUE : FALSE;
    corto_bool urlIterHasNext = corto_iterHasNext(&urlIter) ? TRUE : FALSE;
    while (patternIterHasNext || urlIterHasNext) {
        if (patternIterHasNext != urlIterHasNext) {
            matches = FALSE;
            break;
        }
        char* patternToken = corto_iterNext(&patternIter);
        char* urlToken = corto_iterNext(&urlIter);
        if (patternToken[0] == ':') {
            /* Not doing check for repeated parameters here */
            char* paramName = patternToken + 1;
            corto_rbtreeSet(map, corto_strdup(paramName), corto_strdup(urlToken));
        } else if (strcmp(patternToken, urlToken)) {
            matches = FALSE;
            break;
        }
        patternIterHasNext = corto_iterHasNext(&patternIter) ? TRUE : FALSE;
        urlIterHasNext = corto_iterHasNext(&urlIter) ? TRUE : FALSE;
    }
    if (!matches) {
        corto_rbtreeFree(map);
        map = NULL;
    }
    server_UrlMatch urlMatch = server_UrlMatchCreate(matches, map);
    return urlMatch;

errorNewMap:
errorUrlTokens:
    corto_dealloc(patternTokens);
errorPatternTokens:
    return NULL;
/* $end */
}

corto_string _server_random(
    corto_uint16 n)
{
/* $begin(corto/web/server/random) */
    static char *alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static char *alphanum = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
    corto_uint16 i;
    corto_string result = corto_alloc(n + 1);

    result[0] = alpha[rand() % strlen(alpha)];
    for (i = 1; i < n; i++) {
        result[i] = alphanum[rand() % strlen(alphanum)];
    }

    result[i] = '\0';

    return result;
/* $end */
}

int serverMain(int argc, char* argv[]) {
/* $begin(main) */
    /* Insert code that must be run when component is loaded */
    CORTO_UNUSED(argc);
    CORTO_UNUSED(argv);
    return 0;
/* $end */
}
