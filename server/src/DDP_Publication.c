/* $CORTO_GENERATED
 *
 * DDP_Publication.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

corto_int16 _server_DDP_Publication_create(server_DDP_Publication this, corto_object session, corto_object sub) {
/* $begin(corto/web/server/DDP/Publication/create) */
    server_DDP_Session _session = server_DDP_Session(session);
    server_DDP_Subscription _sub = server_DDP_Subscription(sub);
    corto_id scope;

    server_DDP_Collection coll = server_DDP_Session_getCollection(
        _session,
        corto_path(scope, root_o, this->scope, "/"),
        _sub->meta,
        _sub->value,
        _sub->scope);

    if (!coll) {
      goto error;
    }

    server_DDP_Subscription_ready(_sub);

    return 0;
error:
    return -1;
/* $end */
}
