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

/* ::cortex::web::WebSocketConnection::construct() */
cx_int16 web_WebSocketConnection_construct(web_WebSocketConnection _this) {
/* $begin(::cortex::web::WebSocketConnection::construct) */
    CX_UNUSED(_this);
    return 0;
/* $end */
}

/* ::cortex::web::WebSocketConnection::destruct() */
cx_void web_WebSocketConnection_destruct(web_WebSocketConnection _this) {
/* $begin(::cortex::web::WebSocketConnection::destruct) */
    CX_UNUSED(_this);
/* $end */
}

/* ::cortex::web::WebSocketConnection::send(string message) */
cx_void web_WebSocketConnection_send(web_WebSocketConnection _this, cx_string message) {
/* $begin(::cortex::web::WebSocketConnection::send) */
    mg_websocket_printf((struct mg_connection *)_this->conn, WEBSOCKET_OPCODE_TEXT, message);
/* $end */
}
