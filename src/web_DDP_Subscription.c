/* $CORTO_GENERATED
 *
 * web_DDP_Subscription.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#define WEB_DDP_SUBSCRIPTION_READY "{\"msg\":\"ready\",\"subs\":[\"%s\"]}"

web_DDP_Session web_DDP_Subscription_getSession(web_DDP_Subscription this) {
    return web_DDP_Session(corto_parentof(corto_parentof(this)));
}
/* $end */

corto_void _web_DDP_Subscription_ready(web_DDP_Subscription this) {
/* $begin(corto/web/DDP/Subscription/ready) */
    web_DDP_Session session = web_DDP_Subscription_getSession(this);
    corto_string msg = NULL;

    corto_asprintf(&msg, WEB_DDP_SUBSCRIPTION_READY, this->id);

    web_HTTP_Connection_write(session->conn, msg);

    corto_dealloc(msg);

/* $end */
}
