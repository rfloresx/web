/* $CORTO_GENERATED
 *
 * HTTP_Request.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */
#include "wshtp_server.h"

bool http_parse_header(const char *header, const char *key, char * out, size_t out_len)
{
    bool found = false;
    size_t size = strlen(key);
    out[0] = '\0';
    char *ptr = (char *)header;
    while (ptr && *ptr != '\0') {
        if (strncmp(ptr, key, size) == 0) {
            ptr = ptr + size;
            if ( *ptr == '=') {
                ptr++;
                size = strlen(ptr);
                char *end = strchr(ptr, ';');
                if (end) {
                    size = end - ptr;
                }
                if (size < out_len) {
                    memcpy(out, ptr, size);
                    out[size] = '\0';
                }
                found = true;
                ptr = NULL;
                break;
            }
        }
        ptr = strchr(ptr, ';');
        if (ptr) {
            ptr++;
            while (*ptr == ' '){
                ptr++;
            }
        }
    }
    return found;
}

/* $end */

corto_void _server_HTTP_Request_badRequest(
    server_HTTP_Request* this,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/Request/badRequest) */
    wshtp_conn_t *conn = (wshtp_conn_t*)this->conn;
    wshtp_set_code(conn, 400);
    wshtp_add_header(conn, "Content-Type", "text/html; charset=UTF-8");
    wshtp_send_text(conn, msg);
    corto_error("[HTTP] error: %s", msg);
/* $end */
}

corto_string _server_HTTP_Request_getCookie(
    server_HTTP_Request* this,
    corto_string key)
{
/* $begin(corto/web/server/HTTP/Request/getCookie) */
    wshtp_conn_t *conn = (wshtp_conn_t *)this->conn;
    const char *cookies = wshtp_request_get_header(conn, "Cookie");
    if (cookies == NULL) {
        goto errorNoCookies;
    }
    /* TODO prefer dynamic buffer */
    char buffer[200];

    if (http_parse_header(cookies, key, buffer, sizeof(buffer)) == 0) {
        goto errorParseHeader;
    }

    char* result = corto_strdup(buffer);
    if (result == NULL) {
        goto errorStrdup;
    }

    return result;

errorStrdup:
errorParseHeader:
errorNoCookies:
    return NULL;
/* $end */
}

corto_string _server_HTTP_Request_getVar(
    server_HTTP_Request* this,
    corto_string id)
{
/* $begin(corto/web/server/HTTP/Request/getVar) */
    char *value;
    value = (char *)wshtp_request_get_var((wshtp_conn_t*)this->conn, id);
    if (value == NULL) {
        value = "";
    }
    return value;
/* $end */
}

corto_void _server_HTTP_Request_reply(
    server_HTTP_Request* this,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/Request/reply) */
    wshtp_send_text((wshtp_conn_t*)this->conn, msg);
/* $end */
}

corto_void _server_HTTP_Request_sendfile(
    server_HTTP_Request* this,
    corto_string file)
{
/* $begin(corto/web/server/HTTP/Request/sendfile) */
    wshtp_send_file((wshtp_conn_t*)this->conn, file);
    this->file = TRUE;
/* $end */
}

corto_void _server_HTTP_Request_setCookie(
    server_HTTP_Request* this,
    corto_string key,
    corto_string value)
{
/* $begin(corto/web/server/HTTP/Request/setCookie) */
    wshtp_conn_t *conn = (wshtp_conn_t*)this->conn;

    char* val = NULL;
    corto_asprintf(&val, "%s=%s", key, value);
    /* TODO memory management */
    if (val == NULL) {
        goto error;
    }
    wshtp_add_header(conn, "Set-Cookie", val);
    corto_dealloc(val);
error:;
/* $end */
}

corto_void _server_HTTP_Request_setHeader(
    server_HTTP_Request* this,
    corto_string name,
    corto_string val)
{
/* $begin(corto/web/server/HTTP/Request/setHeader) */
    wshtp_add_header((wshtp_conn_t *)this->conn, name, val);
/* $end */
}

corto_void _server_HTTP_Request_setStatus(
    server_HTTP_Request* this,
    corto_uint16 status)
{
/* $begin(corto/web/server/HTTP/Request/setStatus) */
    wshtp_set_code((wshtp_conn_t *)this->conn, status);
/* $end */
}
