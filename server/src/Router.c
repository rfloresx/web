/* $CORTO_GENERATED
 *
 * Router.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

corto_int16 _server_Router_construct(
    server_Router this)
{
/* $begin(corto/web/server/Router/construct) */
    corto_setref(&corto_interface(this)->base, corto_interface(server_RouteService_o));
    if (corto_class_construct(this)) {
        goto errorSuper;
    }
    return 0;
errorSuper:
    return -1;
/* $end */
}
