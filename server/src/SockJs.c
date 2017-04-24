/* $CORTO_GENERATED
 *
 * SockJs.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */
#include "corto/fmt/json/json.h"
#define SERVER_SOCKJSSERVER_DEFAULT_HEARTBEAT_TIMEOUT  (60)
/* $end */

corto_int16 _server_SockJs_construct(
    server_SockJs this)
{
/* $begin(corto/web/server/SockJs/construct) */
    return server_Service_construct(this);
/* $end */
}

corto_void _server_SockJs_onClose_v(
    server_SockJs this,
    server_HTTP_Connection c)
{
/* $begin(corto/web/server/SockJs/onClose) */

    corto_trace("SockJS: close");

/* $end */
}

corto_void _server_SockJs_onData_v(
    server_SockJs this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/SockJs/onData) */

    /* virtual method */

/* $end */
}

corto_void _server_SockJs_onMessage(
    server_SockJs this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/SockJs/onMessage) */

    /* Parse & unpack message */
    JSON_Value *root = json_parse_string(msg);
    if (!root) {
        goto error;
    }

    if (json_value_get_type(root) == JSONArray) {
        JSON_Array *messages = json_value_get_array(root);
        corto_uint32 i;
        for (i = 0; i < json_array_get_count(messages); i++) {
            const char *message = json_array_get_string(messages, i);
            corto_trace("SockJS: recv %s", message);
            server_SockJs_onData(this, c, (corto_string)message);
        }
    }

    json_value_free(root);

error:;
/* $end */
}

corto_void _server_SockJs_onOpen_v(
    server_SockJs this,
    server_HTTP_Connection c)
{
/* $begin(corto/web/server/SockJs/onOpen) */

    corto_trace("SockJS: open");
    server_HTTP_write(server_Service(this)->server, c, "o");

/* $end */
}

corto_void _server_SockJs_onPoll_v(
    server_SockJs this)
{
/* $begin(corto/web/server/SockJs/onPoll) */
    this->timeElapsed += server_Service(this)->server->pollInterval;

    /* Send heartbeats for all live connections every n seconds */
    if (this->timeElapsed >= (SERVER_SOCKJSSERVER_DEFAULT_HEARTBEAT_TIMEOUT * 1000)) {

        corto_debug("SockJS: heartbeat");
        server_HTTP_broadcast(server_Service(this)->server, "h");

        this->timeElapsed = 0;
    }
/* $end */
}

corto_int16 _server_SockJs_onRequest(
    server_SockJs this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/SockJs/onRequest) */

    if (!strcmp(uri, "info")) {
        corto_string msg;
        corto_asprintf(&msg, "{\"websocket\":true,\"origins\":[\"*:*\"],"
                             "\"cookie_needed\":false,\"entropy\":%u}",
                             10000000000 * rand());
        server_HTTP_Request_setStatus(r, 200);
        server_HTTP_Request_setHeader(r, "Access-Control-Allow-Credentials", "true");
        server_HTTP_Request_setHeader(r, "Access-Control-Allow-Origin", "null");
        server_HTTP_Request_setHeader(r, "Cache-Control", "no-store, no-cache, no-transform, must-revalidate, max-age=0");
        server_HTTP_Request_setHeader(r, "Connection", "keep-alive");
        server_HTTP_Request_setHeader(r, "Content-Type", "application/json; charset=UTF-8");
        server_HTTP_Request_reply(r, msg);

        corto_trace("SockJS: info %s", msg);
        corto_dealloc(msg);

        return 1;

    } else if (!*uri) {
        corto_string msg;

        corto_asprintf(&msg, "Welcome to SockJS!\n",
                             10000000000 * rand());
        server_HTTP_Request_setStatus(r, 200);
        server_HTTP_Request_setHeader(r, "Access-Control-Allow-Origin", "*");
        server_HTTP_Request_setHeader(r, "Content-Type", "text/plain;charset=UTF-8");
        server_HTTP_Request_reply(r, msg);

        corto_trace("SockJS: Welcome to SockJS!");
        corto_dealloc(msg);

        return 1;

    } else if (!strcmp(uri, "iframe.html")) {
        server_HTTP_Request_setStatus(r, 200);
        server_HTTP_Request_reply(r,
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
            "  <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n"
            "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
            "  <script>\n"
            "    document.domain = document.domain;\n"
            "    _sockjs_onload = function(){SockJS.bootstrap_iframe();};\n"
            "  </script>\n"
            "  <script src=\"js/sockjs.min.js\"></script>\n"
            "</head>\n"
            "<body>\n"
            "  <h2>Don't panic!</h2>\n"
            "  <p>This is a SockJS hidden iframe. It's used for cross domain magic.</p>\n"
            "</body>\n"
            "</html>\n");
        return 1;
    } else {
        corto_warning("SockJS: unknown request '%s'", uri);
        return 0;
    }

/* $end */
}

corto_void _server_SockJs_write(
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/SockJs/write) */
    int escapedLength;
    corto_string sockJsMsg;

    /* Escape & pack message in SockJS header */
    escapedLength = stresc(NULL, 0, msg);
    sockJsMsg = corto_alloc(escapedLength + strlen("a[\"\"]") + 1);
    sprintf(sockJsMsg, "a[\"%*s\"]", escapedLength, "");
    stresc(sockJsMsg + 3, escapedLength, msg);

    if (c) {
      corto_trace("SockJS: send %s", msg);
      server_HTTP_Connection_write(c, sockJsMsg);
    } else {
        corto_error("SockJS: send 'null' passed as connection");
    }

    corto_dealloc(sockJsMsg);

/* $end */
}
