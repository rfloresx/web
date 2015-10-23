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

/* ::corto::web::SockJsServer::UriRequest::getVar(string id) */
corto_string _web_SockJsServer_UriRequest_getVar(web_SockJsServer_UriRequest *this, corto_string id) {
/* $begin(::corto::web::SockJsServer::UriRequest::getVar) */
	static char value[256];
    mg_get_var((struct mg_connection *)this->conn, id, value, sizeof(value));
    return value;
/* $end */
}

/* ::corto::web::SockJsServer::UriRequest::setHeader(string name,string val) */
corto_void _web_SockJsServer_UriRequest_setHeader(web_SockJsServer_UriRequest *this, corto_string name, corto_string val) {
/* $begin(::corto::web::SockJsServer::UriRequest::setHeader) */
    mg_send_header((struct mg_connection *)this->conn, name, val);
/* $end */
}

/* ::corto::web::SockJsServer::UriRequest::setStatus(uint16 status) */
corto_void _web_SockJsServer_UriRequest_setStatus(web_SockJsServer_UriRequest *this, corto_uint16 status) {
/* $begin(::corto::web::SockJsServer::UriRequest::setStatus) */
    mg_send_status((struct mg_connection *)this->conn, status);
/* $end */
}

/* ::corto::web::SockJsServer::UriRequest::write(string msg) */
corto_void _web_SockJsServer_UriRequest_write(web_SockJsServer_UriRequest *this, corto_string msg) {
/* $begin(::corto::web::SockJsServer::UriRequest::write) */
    mg_printf_data((struct mg_connection *)this->conn, "%s", msg);
/* $end */
}
