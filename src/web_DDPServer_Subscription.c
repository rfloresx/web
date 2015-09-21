/* web_DDPServer_Subscription.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#define corto_web_LIB
#include "web.h"

/* $header() */
#define WEB_DDPSERVER_SUBSCRIPTION_READY "{\"msg\":\"ready\",\"subs\":[\"%s\"]}"

web_DDPServer_Session web_DDPServer_Subscription_getSession(web_DDPServer_Subscription this) {
    return web_DDPServer_Session(cx_parentof(cx_parentof(this)));
}
/* $end */

/* ::corto::web::DDPServer::Subscription::ready() */
cx_void _web_DDPServer_Subscription_ready(web_DDPServer_Subscription this) {
/* $begin(::corto::web::DDPServer::Subscription::ready) */
    web_DDPServer_Session session = web_DDPServer_Subscription_getSession(this);
    cx_string msg = NULL;
    cx_uint32 msgLength = 0;

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDPSERVER_SUBSCRIPTION_READY,
            this->id);

    msg = cx_alloc(msgLength + 1);

    snprintf(msg, msgLength + 1, WEB_DDPSERVER_SUBSCRIPTION_READY,
            this->id);

    web_SockJsServer_Connection_send(session->conn, msg);

    cx_dealloc(msg);
/* $end */
}
