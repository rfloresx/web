/* $CORTO_GENERATED
 *
 * web_SockJs.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "parson.h"
#define WEB_SOCKJSSERVER_DEFAULT_HEARTBEAT_TIMEOUT  (25)
/* $end */

corto_int16 _web_SockJs_construct(web_SockJs this) {
/* $begin(corto/web/SockJs/construct) */

    return web_Service_construct(this);

/* $end */
}

corto_void _web_SockJs_onClose(web_SockJs this, web_HTTP_Connection c) {
/* $begin(corto/web/SockJs/onClose) */

    /* << Insert implementation >> */

/* $end */
}

corto_void _web_SockJs_onData_v(web_SockJs this, web_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/SockJs/onData) */

    /* virtual method */

/* $end */
}

corto_void _web_SockJs_onMessage(web_SockJs this, web_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/SockJs/onMessage) */

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
            web_SockJs_onData(this, c, (corto_string)message);
        }
    }

    json_value_free(root);

error:;
/* $end */
}

corto_void _web_SockJs_onOpen(web_SockJs this, web_HTTP_Connection c) {
/* $begin(corto/web/SockJs/onOpen) */

    web_HTTP_write(web_Service(this)->server, c, "o");

/* $end */
}

corto_void _web_SockJs_onPoll_v(web_SockJs this) {
/* $begin(corto/web/SockJs/onPoll) */
    this->timeElapsed += web_Service(this)->server->pollInterval;

    /* Send heartbeats for all live connections every n seconds */
    if (this->timeElapsed >= (WEB_SOCKJSSERVER_DEFAULT_HEARTBEAT_TIMEOUT * 1000)) {

        web_HTTP_broadcast(web_Service(this)->server, "h");

        this->timeElapsed = 0;
    }
/* $end */
}

corto_int16 _web_SockJs_onRequest(web_SockJs this, web_HTTP_Connection c, web_HTTP_Request *r) {
/* $begin(corto/web/SockJs/onRequest) */

    if (!strcmp(r->uri, "/sockjs/info")) {
        corto_string msg;
        corto_asprintf(&msg, "{\"websocket\":true,\"origins\":[\"*:*\"],"
                             "\"cookie_needed\":false,\"entropy\":%u}",
                             10000000000 * rand());
        web_HTTP_Request_setHeader(r, "Access-Control-Allow-Origin", "*");
        web_HTTP_Request_reply(r, msg);
        corto_dealloc(msg);

        return 1;
    } else {
        return 0;
    }

/* $end */
}

corto_void _web_SockJs_write(web_HTTP_Connection c, corto_string msg) {
/* $begin(corto/web/SockJs/write) */
    int escapedLength;
    corto_string sockJsMsg;

    /* Escape & pack message in SockJS header */
    escapedLength = stresc(NULL, 0, msg);
    sockJsMsg = corto_alloc(escapedLength + strlen("a[\"\"]") + 1);
    sprintf(sockJsMsg, "a[\"%*s\"]", escapedLength, " ");
    stresc(sockJsMsg + 3, escapedLength, msg);

    if (c) {
      web_HTTP_Connection_write(c, sockJsMsg);
    }

    corto_dealloc(sockJsMsg);

/* $end */
}
