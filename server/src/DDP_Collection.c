/* $CORTO_GENERATED
 *
 * DDP_Collection.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */
#include "json.h"

#define SERVER_DDP_COLLECTION_ADDED "{\"msg\":\"added\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define SERVER_DDP_COLLECTION_CHANGED "{\"msg\":\"changed\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define SERVER_DDP_COLLECTION_REMOVED "{\"msg\":\"removed\",\"collection\":\"%s\",\"id\":\"%s\"}"

server_DDP_Session server_DDP_Collection_getSession(server_DDP_Collection this) {
    return server_DDP_Session(corto_parentof(corto_parentof(this)));
}

corto_string server_DDP_Collection_getJson(
    server_DDP_Collection this,
    corto_object observable)
{
    corto_string meta = NULL, value = NULL, msg = NULL;

    if (this->meta) {
        corto_asprintf(
          &meta,
          "{\"meta\":{\"type\":\"%s\"}}",
          corto_fullpath(NULL, observable));
    }

    if (this->value) {
        corto_string json = json_serialize(observable);
        corto_asprintf(
          &value,
          "{\"value\":%s}",
          json);
        corto_dealloc(json);
    }

    corto_asprintf(
        &msg,
        "{%s%s%s}",
        meta ? meta : "",
        meta ? ", " : "",
        value ? value : ""
    );

    if (meta) corto_dealloc(meta);
    if (value) corto_dealloc(value);

    return msg;
}
/* $end */

corto_int16 _server_DDP_Collection_construct(server_DDP_Collection this) {
/* $begin(corto/web/server/DDP/Collection/construct) */

    server_DDP server = server_DDP(
      corto_parentof(
        corto_parentof(
          corto_parentof(
            corto_parentof(this)))));

    corto_listen(this, server_DDP_Collection_onAdded_o, 0, this->obj, server);
    corto_listen(this, server_DDP_Collection_onChanged_o, 0, this->obj, server);
    corto_listen(this, server_DDP_Collection_onRemoved_o, 0, this->obj, server);

    return 0;

/* $end */
}

corto_void _server_DDP_Collection_onAdded(server_DDP_Collection this, corto_object observable) {
/* $begin(corto/web/server/DDP/Collection/onAdded) */
    server_DDP_Session session = server_DDP_Collection_getSession(this);
    corto_string json = server_DDP_Collection_getJson(this, observable);
    corto_string msg;
    corto_asprintf(
        &msg,
        SERVER_DDP_COLLECTION_ADDED,
        corto_nameof(this->obj),
        corto_fullpath(NULL, observable),
        json);


    server_SockJs_write(session->conn, msg);

    corto_dealloc(msg);
    corto_dealloc(json);
/* $end */
}

corto_void _server_DDP_Collection_onChanged(server_DDP_Collection this, corto_object observable) {
/* $begin(corto/web/server/DDP/Collection/onChanged) */
    server_DDP_Session session = server_DDP_Collection_getSession(this);
    corto_string json = server_DDP_Collection_getJson(this, observable);
    corto_string msg;
    corto_asprintf(
        &msg,
        SERVER_DDP_COLLECTION_CHANGED,
        corto_nameof(this->obj),
        corto_fullpath(NULL, observable),
        json);


    server_SockJs_write(session->conn, msg);

    corto_dealloc(msg);
    corto_dealloc(json);
/* $end */
}

corto_void _server_DDP_Collection_onRemoved(server_DDP_Collection this, corto_object observable) {
/* $begin(corto/web/server/DDP/Collection/onRemoved) */
    server_DDP_Session session = server_DDP_Collection_getSession(this);
    corto_string msg;
    corto_asprintf(
        &msg,
        SERVER_DDP_COLLECTION_REMOVED,
        corto_nameof(this->obj),
        corto_fullpath(NULL, observable));

    server_SockJs_write(session->conn, msg);

    corto_dealloc(msg);

/* $end */
}

corto_void _server_DDP_Collection_subscribe(server_DDP_Collection this, corto_bool value, corto_bool meta, corto_bool scope) {
/* $begin(corto/web/server/DDP/Collection/subscribe) */

    this->value += value ? 1 : 0;
    this->meta += meta ? 1 : 0;
    this->scope += scope ? 1 : 0;

/* $end */
}

corto_void _server_DDP_Collection_unsubscribe(server_DDP_Collection this, corto_bool value, corto_bool meta, corto_bool scope) {
/* $begin(corto/web/server/DDP/Collection/unsubscribe) */

    this->value -= value ? 1 : 0;
    this->meta -= meta ? 1 : 0;
    this->scope -= scope ? 1 : 0;

/* $end */
}
