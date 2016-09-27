/* $CORTO_GENERATED
 *
 * StandaloneHTTPS.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

corto_int16 _server_StandaloneHTTPS_construct(
    server_StandaloneHTTPS this)
{
/* $begin(corto/web/server/StandaloneHTTPS/construct) */
    server_StandaloneHTTP(this)->enable_ssl = 1;
    return server_StandaloneHTTP_construct(server_StandaloneHTTP(this));
/* $end */
}

corto_void _server_StandaloneHTTPS_destruct(
    server_StandaloneHTTPS this)
{
/* $begin(corto/web/server/StandaloneHTTPS/destruct) */
    server_StandaloneHTTP_destruct(server_StandaloneHTTP(this));
/* $end */
}
