/* web_WebSocketConnection.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#define corto_web_LIB
#include "web.h"

/* $header() */
#include "mongoose.h"
/* $end */

/* ::corto::web::WebSocketConnection::send(string message) */
cx_void _web_WebSocketConnection_send(web_WebSocketConnection this, cx_string message) {
/* $begin(::corto::web::WebSocketConnection::send) */
    mg_websocket_printf((struct mg_connection *)this->conn, WEBSOCKET_OPCODE_TEXT, message);
/* $end */
}
