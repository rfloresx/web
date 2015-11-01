/* $CORTO_GENERATED
 *
 * web_DDPServer_Publication.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

corto_int16 _web_DDPServer_Publication_create(web_DDPServer_Publication this, corto_object session, corto_object sub) {
/* $begin(::corto::web::DDPServer::Publication::create) */
	web_DDPServer_Session _session = web_DDPServer_Session(session);
	web_DDPServer_Subscription _sub = web_DDPServer_Subscription(sub);

	web_DDPServer_Collection coll = web_DDPServer_Session_getCollection(_session, corto_nameof(this->scope));
	if (!coll) {
		goto error;
	}

	web_DDPServer_Subscription_ready(_sub);

	web_DDPServer_Collection_subscribe(coll, _sub->value, _sub->meta, _sub->scope);
	
	return 0;
error:
	return -1;
/* $end */
}
