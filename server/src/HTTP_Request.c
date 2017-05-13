/* $CORTO_GENERATED
 *
 * HTTP_Request.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */

static
corto_bool parseCookie(const char *header, const char *key, char * out, size_t *out_len)
{
    corto_bool found = FALSE;
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
                if (size < *out_len) {
                    memcpy(out, ptr, size);
                    out[size] = '\0';
                } else {
                    *out_len = size + 1;
                }
                found = TRUE;
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

void _server_HTTP_Request_badRequest(
    server_HTTP_Request* this,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/Request/badRequest) */
    server_HTTP_Request_setStatus(this, 400);
    server_HTTP_Request_setHeader(this, "Content-Type", "text/html; charset=UTF-8");
    server_HTTP_Request_reply(this, msg);
/* $end */
}

corto_string _server_HTTP_Request_getCookie(
    server_HTTP_Request* this,
    corto_string key)
{
/* $begin(corto/web/server/HTTP/Request/getCookie) */
    size_t size = 0;
    char *result = NULL;
    char *header = server_HTTP_Request_getHeader(this, "Cookie");

    if (parseCookie(header, key, result, &size)) {
        result = corto_alloc(size);
        corto_assert(parseCookie(header, key, result, &size), "cookie disappeared :(");
    }

    return result;
/* $end */
}

corto_string _server_HTTP_Request_getHeader(
    server_HTTP_Request* this,
    corto_string key)
{
/* $begin(corto/web/server/HTTP/Request/getHeader) */
    corto_string result;
    server_HTTP_Request_d_getHeaderCall(&this->m_getHeader, &result, this, key);
    return result;
/* $end */
}

corto_string _server_HTTP_Request_getVar(
    server_HTTP_Request* this,
    corto_string key)
{
/* $begin(corto/web/server/HTTP/Request/getVar) */
    corto_string result;
    server_HTTP_Request_d_getVarCall(&this->m_getVar, &result, this, key);
    return result;
/* $end */
}

void _server_HTTP_Request_reply(
    server_HTTP_Request* this,
    corto_string msg)
{
/* $begin(corto/web/server/HTTP/Request/reply) */
    server_HTTP_Request_d_replyCall(&this->m_reply, this, msg);
/* $end */
}

void _server_HTTP_Request_sendfile(
    server_HTTP_Request* this,
    corto_string file)
{
/* $begin(corto/web/server/HTTP/Request/sendfile) */
    server_HTTP_Request_d_sendFileCall(&this->m_sendFile, this, file);
/* $end */
}

void _server_HTTP_Request_setCookie(
    server_HTTP_Request* this,
    corto_string key,
    corto_string value)
{
/* $begin(corto/web/server/HTTP/Request/setCookie) */
    char *cookie;
    corto_asprintf(&cookie, "%s=%s", key, value);
    server_HTTP_Request_setHeader(this, "Set-Cookie", cookie);
    corto_dealloc(cookie);
/* $end */
}

void _server_HTTP_Request_setHeader(
    server_HTTP_Request* this,
    corto_string key,
    corto_string val)
{
/* $begin(corto/web/server/HTTP/Request/setHeader) */
    server_HTTP_Request_d_setHeaderCall(&this->m_setHeader, this, key, val);
/* $end */
}

void _server_HTTP_Request_setStatus(
    server_HTTP_Request* this,
    uint16_t status)
{
/* $begin(corto/web/server/HTTP/Request/setStatus) */
    server_HTTP_Request_d_setStatusCall(&this->m_setStatus, this, status);
/* $end */
}
