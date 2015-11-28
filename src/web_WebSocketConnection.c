/* $CORTO_GENERATED
 *
 * web_WebSocketConnection.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "mongoose.h"
/* $end */

corto_void _web_WebSocketConnection_send(web_WebSocketConnection this, corto_string message) {
/* $begin(corto/web/WebSocketConnection/send) */
    mg_websocket_printf((struct mg_connection *)this->conn, WEBSOCKET_OPCODE_TEXT, message);
/* $end */
}
