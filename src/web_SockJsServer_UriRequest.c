/* web_SockJsServer_UriRequest.c
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

/* ::cortex::web::SockJsServer::UriRequest::getVar(string id) */
cx_string web_SockJsServer_UriRequest_getVar(web_SockJsServer_UriRequest *_this, cx_string id) {
/* $begin(::cortex::web::SockJsServer::UriRequest::getVar) */
	static char value[256];
    mg_get_var((struct mg_connection *)_this->conn, id, value, sizeof(value));
    return value;
/* $end */
}

/* ::cortex::web::SockJsServer::UriRequest::setHeader(string name,string val) */
cx_void web_SockJsServer_UriRequest_setHeader(web_SockJsServer_UriRequest *_this, cx_string name, cx_string val) {
/* $begin(::cortex::web::SockJsServer::UriRequest::setHeader) */
    mg_send_header((struct mg_connection *)_this->conn, name, val);
/* $end */
}

/* ::cortex::web::SockJsServer::UriRequest::setStatus(uint16 status) */
cx_void web_SockJsServer_UriRequest_setStatus(web_SockJsServer_UriRequest *_this, cx_uint16 status) {
/* $begin(::cortex::web::SockJsServer::UriRequest::setStatus) */
    mg_send_status((struct mg_connection *)_this->conn, status);
/* $end */
}

/* ::cortex::web::SockJsServer::UriRequest::write(string msg) */
cx_void web_SockJsServer_UriRequest_write(web_SockJsServer_UriRequest *_this, cx_string msg) {
/* $begin(::cortex::web::SockJsServer::UriRequest::write) */
    mg_printf_data((struct mg_connection *)_this->conn, "%s", msg);
/* $end */
}
