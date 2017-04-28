/* $CORTO_GENERATED
 *
 * Files.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

int16_t _server_Files_construct(
    server_Files this)
{
/* $begin(corto/web/server/Files/construct) */
    if (!this->path) {
        corto_setstr(&this->path, ".");
    }
    return server_Service_construct(this);
/* $end */
}

int16_t _server_Files_onRequest(
    server_Files this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/Files/onRequest) */
    corto_string file;
    corto_string request = uri;

    if (!uri || !strlen(uri) || !strcmp(uri, "/")) {
        request = "index.html";
    }

    corto_asprintf(
        &file,
        "%s/%s",
        this->path,
        request);

    if (corto_fileTest("%s", file)) {
        corto_trace("Files: serving '%s'", file);
        server_HTTP_Request_sendfile(r, file);
    } else {
        corto_string msg;
        corto_asprintf(&msg, "Resource '%s' not found", uri);
        server_HTTP_Request_setStatus(r, 404);
        server_HTTP_Request_reply(r, msg);
        corto_dealloc(msg);
    }
    corto_dealloc(file);

    return 1;
/* $end */
}
