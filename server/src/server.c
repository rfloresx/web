/* $CORTO_GENERATED
 *
 * server.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */

#include "mongoose.h"

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

static void server_queryToMapExt_0(
    const char* content,
    size_t contentLen,
    corto_int8* state,
    const char** last,
    const char* curr,
    char** key,
    char** value,
    corto_rbtree params)
{
    corto_bool reachedEnd = curr == content + contentLen;
    if (reachedEnd) {
        *state = 3;
        return;
    }

    char c = *curr;
    *last = curr;
    if (c == '&' || c == '=') {
        *state = -1;
    } else {
        *state = 1;
    }
}

static void server_queryToMapExt_1(
    const char* content,
    size_t contentLen,
    corto_int8* state,
    const char** last,
    const char* curr,
    char** key,
    char** value,
    corto_rbtree params)
{
    corto_bool reachedEnd = curr == content + contentLen;
    char c = '\0';
    if (reachedEnd || (c = *curr) == '&') {
        *state = -1;
        return;
    }
    if (c == '=') {
        size_t len = curr - *last;
        *key = corto_alloc(len + 1);
        strncpy(*key, *last, len);
        (*key)[len] = '\0';
        *state = 2;
        *last = curr + 1;
    }
}

static void server_queryToMapExt_2(
    const char* content,
    size_t contentLen,
    corto_int8* state,
    const char** last,
    const char* curr,
    char** key,
    char** value,
    corto_rbtree params)
{
    char c = *curr;
    if (c == '=') {
        *state = -1;
        return;
    }

    corto_bool reachedEnd = curr == content + contentLen;
    if (reachedEnd || c == '&') {
        size_t len = curr - *last;
        *value = corto_alloc(len + 1);
        strncpy(*value, *last, len);
        (*value)[len] = '\0';

        char* decodedValue = server_urlDecode(*value);
        corto_dealloc(*value);
        corto_rbtreeSet(params, *key, decodedValue);
        *key = NULL;
        *value = NULL;
    }

    if (reachedEnd) {
        *state = 3;
    } else if (c == '&') {
        *state = 0;
    }
}

static corto_equalityKind server_HTTP_Request_compareString(corto_type this, const void* o1, const void* o2)
{
    CORTO_UNUSED(this);
    const char* i1 = o1;
    const char* i2 = o2;
    corto_equalityKind result = CORTO_EQ;
    while (result == CORTO_EQ && *i1 && *i2) {
        result = *i1 == *i2 ? CORTO_EQ : (*i1 > *i2 ? CORTO_GT : CORTO_LT);
        i1++;
        i2++;
    }
    return result;
}

static corto_rbtree server_queryToMapExt(
    const char* query,
    size_t queryLength)
{
    corto_rbtree params = corto_rbtreeNew_w_func(server_HTTP_Request_compareString);
    if (params == NULL) {
        goto errorRbtreeNew;
    }

    corto_int8 state = 0;
    const char* last = NULL;
    const char* curr = query;
    char* key = NULL;
    char* value = NULL;

    while (state != 3) {
        switch (state) {
        case 0:
            server_queryToMapExt_0(
                query, queryLength, &state, &last, curr, &key, &value, params
            );
            break;
        case 1:
            server_queryToMapExt_1(
                query, queryLength, &state, &last, curr, &key, &value, params
            );
            break;
        case 2:
            server_queryToMapExt_2(
                query, queryLength, &state, &last, curr, &key, &value, params
            );
            break;
        default:
            break;
        }
        curr++;
    }

    if (state == -1) {
        corto_rbtreeFree(params);
        params = NULL;
    }

    return params;

errorRbtreeNew:
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

corto_rbtree _server_queryToMap(
    corto_string query)
{
/* $begin(corto/web/server/queryToMap) */
    return query ? server_queryToMapExt(query, strlen(query)) : NULL;
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

/* $header(corto/web/server/typedescriptor) */
typedef struct server_typedescriptorSer_t {
    corto_buffer b;
    corto_bool first;
    corto_string prefix;
} server_typedescriptorSer_t;

corto_int16 server_typedescriptorSer_void(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "null");
    return 0;
}
corto_int16 server_typedescriptorSer_primitive(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "%d", corto_primitive(corto_value_getType(info))->kind);
    return 0;
}
corto_int16 server_typedescriptorSer_reference(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "9");
    return 0;
}
corto_int16 server_typedescriptorSer_object(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    if (corto_value_getType(info)->kind == CORTO_PRIMITIVE) {
        corto_buffer_appendstr(&data->b, "{\"value\":");
    }

    corto_int16 result = corto_serializeValue(s, info, userData);

    if (corto_value_getType(info)->kind == CORTO_PRIMITIVE) {
        corto_buffer_appendstr(&data->b, "}");
    }
    return result;
}
corto_int16 server_typedescriptorSer_member(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_member m = info->is.member.t;
    if (!data->first) {
        corto_buffer_appendstr(&data->b, ",");
    } else {
        data->first = FALSE;
    }
    corto_buffer_append(&data->b, "\"%s\":", corto_idof(m));
    return corto_serializeValue(s, info, userData);
}
corto_int16 server_typedescriptorSer_base(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "\"super\":");
    data->first = FALSE;
    return corto_serializeValue(s, info, userData);
}
corto_int16 server_typedescriptorSer_composite(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_appendstr(&data->b, "{");
    corto_bool prev = data->first;
    data->first = TRUE;
    corto_int16 result = corto_serializeMembers(s, info, userData);
    data->first = prev;
    corto_buffer_appendstr(&data->b, "}");
    return result;
}
corto_int16 server_typedescriptorSer_collection(
    corto_serializer s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_appendstr(&data->b, "[");
    corto_bool prev = data->first;
    data->first = TRUE;
    corto_int16 result = corto_serializeElements(s, info, userData);
    data->first = prev;
    corto_buffer_appendstr(&data->b, "]");
    return result;
}
struct corto_serializer_s server_typedescriptorSer(void) {
    struct corto_serializer_s s;
    corto_serializerInit(&s);
    s.accessKind = CORTO_NOT;
    s.access = CORTO_PRIVATE;
    s.aliasAction = CORTO_SERIALIZER_ALIAS_IGNORE;
    s.optionalAction = CORTO_SERIALIZER_OPTIONAL_ALWAYS;
    s.program[CORTO_VOID] = server_typedescriptorSer_void;
    s.program[CORTO_PRIMITIVE] = server_typedescriptorSer_primitive;
    s.reference = server_typedescriptorSer_reference;
    s.metaprogram[CORTO_OBJECT] = server_typedescriptorSer_object;
    s.program[CORTO_COMPOSITE] = server_typedescriptorSer_composite;
    s.program[CORTO_COLLECTION] = server_typedescriptorSer_collection;
    s.metaprogram[CORTO_BASE] = server_typedescriptorSer_base;
    s.metaprogram[CORTO_MEMBER] = server_typedescriptorSer_member;
    return s;
}
/* $end */
corto_string _server_typedescriptor(
    corto_type type)
{
/* $begin(corto/web/server/typedescriptor) */
    struct corto_serializer_s s = server_typedescriptorSer();
    server_typedescriptorSer_t walkData = {CORTO_BUFFER_INIT, TRUE};
    corto_metaWalk(&s, type, &walkData);
    return corto_buffer_str(&walkData.b);
/* $end */
}

corto_string _server_urlDecode(
    corto_string s)
{
/* $begin(corto/web/server/urlDecode) */
    size_t len = strlen(s);
    char* dest = corto_alloc(len + 1);
    if (dest == NULL) {
        goto error;
    }
    mg_url_decode(s, len + 1, dest, len + 1, 1);
    return dest;
error:
    return NULL;
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
