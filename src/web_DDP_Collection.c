/* $CORTO_GENERATED
 *
 * web_DDP_Collection.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "json.h"

#define WEB_DDP_COLLECTION_ADDED "{\"msg\":\"added\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define WEB_DDP_COLLECTION_CHANGED "{\"msg\":\"changed\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define WEB_DDP_COLLECTION_REMOVED "{\"msg\":\"removed\",\"collection\":\"%s\",\"id\":\"%s\"}"
#define WEB_DDP_COLLECTION_REMOVE_ALL "{\"msg\":\"removed\",\"collection\":\"%s\"}"

web_DDP_Session web_DDP_Collection_getSession(web_DDP_Collection this) {
    return web_DDP_Session(corto_parentof(corto_parentof(this)));
}
/* $end */

corto_int16 _web_DDP_Collection_construct(web_DDP_Collection this) {
/* $begin(corto/web/DDP/Collection/construct) */
    corto_any thisAny = {corto_typeof(this), this, FALSE};

    web_DDP server = web_DDP(
      corto_parentof(
        corto_parentof(
          corto_parentof(
            corto_parentof(this)))));

    corto_class_setDispatcher(thisAny, web_DDP_Collection_onAdded_o, server);
    corto_class_setDispatcher(thisAny, web_DDP_Collection_onChanged_o, server);
    corto_class_setDispatcher(thisAny, web_DDP_Collection_onRemoved_o, server);

    corto_class_setObservable(thisAny, web_DDP_Collection_onAdded_o, this->obj);
    corto_class_setObservable(thisAny, web_DDP_Collection_onChanged_o, this->obj);
    corto_class_setObservable(thisAny, web_DDP_Collection_onRemoved_o, this->obj);

  return 0;

/* $end */
}

corto_void _web_DDP_Collection_onAdded(web_DDP_Collection this, corto_object observable) {
/* $begin(corto/web/DDP/Collection/onAdded) */
    struct corto_serializer_s serializer =
        corto_json_ser(CORTO_PRIVATE, CORTO_NOT, CORTO_SERIALIZER_TRACE_NEVER);
    web_DDP_Session session = web_DDP_Collection_getSession(this);
    corto_string msg = NULL;
    corto_id id;

    corto_json_ser_t jsonData = {
      NULL, NULL, 0, 0, 0, this->meta, this->value, this->scope, TRUE, FALSE};
    corto_serialize(&serializer, observable, &jsonData);

    corto_asprintf(&msg, WEB_DDP_COLLECTION_ADDED,
        corto_nameof(this->obj),
        corto_fullname(observable, id),
        jsonData.buffer);

    web_SockJs_write(session->conn, msg);

    corto_dealloc(msg);
/* $end */
}

corto_void _web_DDP_Collection_onChanged(web_DDP_Collection this, corto_object observable) {
/* $begin(corto/web/DDP/Collection/onChanged) */
    struct corto_serializer_s serializer =
        corto_json_ser(CORTO_PRIVATE, CORTO_NOT, CORTO_SERIALIZER_TRACE_NEVER);
    web_DDP_Session session = web_DDP_Collection_getSession(this);
    corto_string msg = NULL;
    corto_id id;

    corto_json_ser_t jsonData =
        {NULL, NULL, 0, 0, 0, this->meta, this->value, this->scope, TRUE, FALSE};
    corto_serialize(&serializer, observable, &jsonData);

    /* Create message */
    corto_asprintf(&msg, WEB_DDP_COLLECTION_CHANGED,
        corto_nameof(this->obj),
        corto_fullname(observable, id),
        jsonData.buffer);

    web_SockJs_write(session->conn, msg);

    corto_dealloc(msg);

/* $end */
}

corto_void _web_DDP_Collection_onRemoved(web_DDP_Collection this, corto_object observable) {
/* $begin(corto/web/DDP/Collection/onRemoved) */
    web_DDP_Session session = web_DDP_Collection_getSession(this);
    corto_string msg = NULL;
    corto_uint32 msgLength = 0;
    corto_id id;

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDP_COLLECTION_REMOVED,
            corto_nameof(this->obj),
            corto_fullname(observable, id));

    msg = corto_alloc(msgLength + 1);

    snprintf(msg, msgLength + 1, WEB_DDP_COLLECTION_REMOVED,
            corto_nameof(this->obj),
            id);

    web_SockJs_write(session->conn, msg);

    corto_dealloc(msg);

/* $end */
}

corto_void _web_DDP_Collection_subscribe(web_DDP_Collection this, corto_bool value, corto_bool meta, corto_bool scope) {
/* $begin(corto/web/DDP/Collection/subscribe) */

    this->value += value ? 1 : 0;
    this->meta += meta ? 1 : 0;
    this->scope += scope ? 1 : 0;

/* $end */
}

corto_void _web_DDP_Collection_unsubscribe(web_DDP_Collection this, corto_bool value, corto_bool meta, corto_bool scope) {
/* $begin(corto/web/DDP/Collection/unsubscribe) */

    this->value -= value ? 1 : 0;
    this->meta -= meta ? 1 : 0;
    this->scope -= scope ? 1 : 0;

/* $end */
}
