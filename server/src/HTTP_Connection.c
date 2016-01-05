/* $CORTO_GENERATED
 *
 * HTTP_Connection.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

corto_void _server_HTTP_Connection_write(server_HTTP_Connection this, corto_string msg) {
/* $begin(corto/web/server/HTTP/Connection/write) */

    server_HTTP_write(this->server, this, msg);

/* $end */
}
