/* $CORTO_GENERATED
 *
 * web_DDP_Publication.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

corto_int16 _web_DDP_Publication_create(web_DDP_Publication this, corto_object session, corto_object sub) {
/* $begin(corto/web/DDP/Publication/create) */
    web_DDP_Session _session = web_DDP_Session(session);
    web_DDP_Subscription _sub = web_DDP_Subscription(sub);

    web_DDP_Collection coll = web_DDP_Session_getCollection(
        _session,
        corto_nameof(this->scope),
        _sub->meta,
        _sub->value,
        _sub->scope);

    if (!coll) {
      goto error;
    }

    web_DDP_Subscription_ready(_sub);

    return 0;
error:
    return -1;
/* $end */
}
