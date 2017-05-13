/* $CORTO_GENERATED
 *
 * server.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

corto_string _server_random(
    uint16_t n)
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
    corto_walk_opt* s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "null");
    return 0;
}
corto_int16 server_typedescriptorSer_primitive(
    corto_walk_opt* s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "%d", corto_primitive(corto_value_typeof(info))->kind);
    return 0;
}
corto_int16 server_typedescriptorSer_reference(
    corto_walk_opt* s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "9");
    return 0;
}
corto_int16 server_typedescriptorSer_object(
    corto_walk_opt* s,
    corto_value *info,
    void *userData)
{
    corto_int16 result = corto_value_walk(s, info, userData);
    return result;
}

corto_int16 server_typedescriptorSer_member(
    corto_walk_opt* s,
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
    return corto_value_walk(s, info, userData);
}
corto_int16 server_typedescriptorSer_base(
    corto_walk_opt* s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_append(&data->b, "\"super\":");
    data->first = FALSE;
    return corto_value_walk(s, info, userData);
}
corto_int16 server_typedescriptorSer_composite(
    corto_walk_opt* s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_appendstr(&data->b, "{");
    corto_bool prev = data->first;
    data->first = TRUE;
    corto_int16 result = corto_walk_members(s, info, userData);
    data->first = prev;
    corto_buffer_appendstr(&data->b, "}");
    return result;
}
corto_int16 server_typedescriptorSer_collection(
    corto_walk_opt* s,
    corto_value *info,
    void *userData)
{
    server_typedescriptorSer_t *data = userData;
    corto_buffer_appendstr(&data->b, "[");
    corto_bool prev = data->first;
    data->first = TRUE;
    corto_int16 result = corto_walk_elements(s, info, userData);
    data->first = prev;
    corto_buffer_appendstr(&data->b, "]");
    return result;
}
corto_walk_opt server_typedescriptorSer(void) {
    corto_walk_opt s;
    corto_walk_init(&s);
    s.accessKind = CORTO_NOT;
    s.access = CORTO_PRIVATE;
    s.aliasAction = CORTO_WALK_ALIAS_IGNORE;
    s.optionalAction = CORTO_WALK_OPTIONAL_ALWAYS;
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
    corto_walk_opt s = server_typedescriptorSer();
    server_typedescriptorSer_t walkData = {CORTO_BUFFER_INIT, TRUE};
    corto_metawalk(&s, type, &walkData);
    return corto_buffer_str(&walkData.b);
/* $end */
}

int serverMain(int argc, char *argv[]) {
/* $begin(main) */
    /* Insert code that must be run when component is loaded */
    CORTO_UNUSED(argc);
    CORTO_UNUSED(argv);
    return 0;
/* $end */
}
