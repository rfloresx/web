/* web_DDPServer_Session.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* ::cortex::web::DDPServer::Session::connected() */
cx_void web_DDPServer_Session_connected(web_DDPServer_Session _this) {
/* $begin(::cortex::web::DDPServer::Session::connected) */
    int msgLength = snprintf(NULL, 0, "{\"msg\":\"connected\",\"session\":\"%s\"}", cx_nameof(_this));
    cx_string msg = cx_malloc(msgLength + 1);
    sprintf(msg, "{\"msg\":\"connected\",\"session\":\"%s\"}", cx_nameof(_this));
    web_SockJsServer_Connection_send(_this->conn, msg);
    cx_dealloc(msg);
/* $end */
}

/* ::cortex::web::DDPServer::Session::failed(::cortex::web::SockJsServer::Connection conn) */
cx_void web_DDPServer_Session_failed(web_SockJsServer_Connection conn) {
/* $begin(::cortex::web::DDPServer::Session::failed) */
    web_SockJsServer_Connection_send(conn, "{\"msg\":\"failed\",\"version\":\"1\"}");
/* $end */
}

/* ::cortex::web::DDPServer::Session::onDelete() */
cx_void web_DDPServer_Session_onDelete(web_DDPServer_Session _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::DDPServer::Session::onDelete) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    CX_UNUSED(source);
/* $end */
}

/* ::cortex::web::DDPServer::Session::onUpdate() */
cx_void web_DDPServer_Session_onUpdate(web_DDPServer_Session _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::DDPServer::Session::onUpdate) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    CX_UNUSED(source);
/* $end */
}

/* ::cortex::web::DDPServer::Session::pong(string id) */
cx_void web_DDPServer_Session_pong(web_DDPServer_Session _this, cx_string id) {
/* $begin(::cortex::web::DDPServer::Session::pong) */
    if (id) {
        int msgLength = snprintf(NULL, 0, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        cx_string msg = cx_malloc(msgLength + 1);
        sprintf(msg, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        web_SockJsServer_Connection_send(_this->conn, msg);
        cx_dealloc(msg);
    } else {
        web_SockJsServer_Connection_send(_this->conn, "{\"msg\":\"pong\"}");
    }
/* $end */
}
