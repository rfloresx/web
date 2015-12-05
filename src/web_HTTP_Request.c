/* $CORTO_GENERATED
 *
 * web_HTTP_Request.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "mongoose.h"
/* $end */

corto_string _web_HTTP_Request_getVar(web_HTTP_Request *this, corto_string id) {
/* $begin(corto/web/HTTP/Request/getVar) */
    static char value[256];

    mg_get_var((struct mg_connection *)this->conn, id, value, sizeof(value));

    return value;
/* $end */
}

corto_void _web_HTTP_Request_reply(web_HTTP_Request *this, corto_string msg) {
/* $begin(corto/web/HTTP/Request/reply) */

    mg_printf_data((struct mg_connection *)this->conn, "%s", msg);

/* $end */
}

corto_void _web_HTTP_Request_setHeader(web_HTTP_Request *this, corto_string name, corto_string val) {
/* $begin(corto/web/HTTP/Request/setHeader) */

    mg_send_header((struct mg_connection *)this->conn, name, val);

/* $end */
}

corto_void _web_HTTP_Request_setStatus(web_HTTP_Request *this, corto_uint16 status) {
/* $begin(corto/web/HTTP/Request/setStatus) */

    mg_send_status((struct mg_connection *)this->conn, status);

/* $end */
}
