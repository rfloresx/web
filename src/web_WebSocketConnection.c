/* web_WebSocketConnection.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */
#include "mongoose.h"
/* $end */

/* ::corto::web::WebSocketConnection::send(string message) */
corto_void _web_WebSocketConnection_send(web_WebSocketConnection this, corto_string message) {
/* $begin(::corto::web::WebSocketConnection::send) */
    mg_websocket_printf((struct mg_connection *)this->conn, WEBSOCKET_OPCODE_TEXT, message);
/* $end */
}
