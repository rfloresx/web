/* $CORTO_GENERATED
 *
 * DDP_Subscription.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */
#define SERVER_DDP_SUBSCRIPTION_READY "{\"msg\":\"ready\",\"subs\":[\"%s\"]}"

server_DDP_Session server_DDP_Subscription_getSession(server_DDP_Subscription this) {
    return server_DDP_Session(corto_parentof(corto_parentof(this)));
}
/* $end */

corto_void _server_DDP_Subscription_ready(server_DDP_Subscription this) {
/* $begin(corto/web/server/DDP/Subscription/ready) */
    server_DDP_Session session = server_DDP_Subscription_getSession(this);
    corto_string msg = NULL;

    corto_asprintf(&msg, SERVER_DDP_SUBSCRIPTION_READY, this->id);

    server_HTTP_Connection_write(session->conn, msg);

    corto_dealloc(msg);

/* $end */
}
