/* $CORTO_GENERATED
 *
 * web_DDP_Session.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
corto_void web_DDP_Session_nosub(web_DDP_Session this, corto_string id) {
    web_HTTP server = web_Service(corto_parentof(corto_parentof(this)))->server;
    int msgLength = snprintf(NULL, 0, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);

    corto_string msg = corto_alloc(msgLength + 1);
    sprintf(msg, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);

    web_HTTP_write(server, this->conn, msg);
    corto_dealloc(msg);
}
/* $end */

corto_void _web_DDP_Session_connected(web_DDP_Session this) {
/* $begin(corto/web/DDP/Session/connected) */
    int msgLength = snprintf(
        NULL, 0,
        "{\"msg\":\"connected\",\"session\":\"%s\"}",
        corto_nameof(this));

    corto_string msg = corto_alloc(msgLength + 1);
    sprintf(msg,
        "{\"msg\":\"connected\",\"session\":\"%s\"}", corto_nameof(this));

    web_HTTP_Connection_write(this->conn, msg);
    corto_dealloc(msg);
/* $end */
}

corto_int16 _web_DDP_Session_construct(web_DDP_Session this) {
/* $begin(corto/web/DDP/Session/construct) */

    this->collections = corto_voidCreateChild(this, "__collections");
    this->subscriptions = corto_voidCreateChild(this, "__subscriptions");

    return 0;
/* $end */
}

corto_void _web_DDP_Session_error(web_DDP_Session this, corto_string reason, corto_string offendingMessage) {
/* $begin(corto/web/DDP/Session/error) */
    corto_string msg = NULL;
    if (offendingMessage) {
        corto_asprintf(&msg,
          "{\"msg\":\"error\",\"reason\":\"%s\",\"offendingMessage\":\"%s\"}",
          reason, offendingMessage);
    } else {
        corto_asprintf(&msg,
          "{\"msg\":\"error\",\"reason\":\"%s\"}",
          reason);
    }
    corto_dealloc(msg);
    web_HTTP_Connection_write(this->conn, msg);
/* $end */
}

corto_void _web_DDP_Session_failed(web_HTTP_Connection conn) {
/* $begin(corto/web/DDP/Session/failed) */
    web_HTTP_Connection_write(conn,
        "{\"msg\":\"failed\",\"version\":\"1\"}");
/* $end */
}

web_DDP_Collection _web_DDP_Session_getCollection(web_DDP_Session this, corto_string name) {
/* $begin(corto/web/DDP/Session/getCollection) */
    corto_id collectionName;
    web_DDP_Collection result = NULL;
    sprintf(collectionName, "c_%s", name ? name : "");

    if (!(result = corto_lookup(this->collections, collectionName))) {
        corto_object o = corto_resolve(NULL, name);
        if (o) {
            result = web_DDP_CollectionCreateChild(
                this->collections, collectionName, o, FALSE, FALSE, FALSE);
        }
    }

    return result;
/* $end */
}

web_DDP_Subscription _web_DDP_Session_getSub(web_DDP_Session this, web_DDP_Publication pub, corto_string id, corto_bool meta, corto_bool value, corto_bool scope) {
/* $begin(corto/web/DDP/Session/getSub) */
    web_DDP_Subscription result = NULL;

    if (!(result = corto_lookup(this->subscriptions, id))) {
        result = web_DDP_SubscriptionCreateChild(
            this->subscriptions, id, pub, id, value, meta, scope);
    }

    return result;
/* $end */
}

corto_void _web_DDP_Session_pong(web_DDP_Session this, corto_string id) {
/* $begin(corto/web/DDP/Session/pong) */
    if (id) {
        int msgLength = snprintf(NULL, 0,
            "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        corto_string msg = corto_alloc(msgLength + 1);
        sprintf(msg, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        web_HTTP_Connection_write(this->conn, msg);
        corto_dealloc(msg);
    } else {
        web_HTTP_Connection_write(this->conn, "{\"msg\":\"pong\"}");
    }
/* $end */
}

corto_void _web_DDP_Session_sub(web_DDP_Session this, corto_string id, corto_string name, corto_bool meta, corto_bool value, corto_bool scope) {
/* $begin(corto/web/DDP/Session/sub) */
    web_DDP server = web_DDP(corto_parentof(corto_parentof(this)));

    /* Find matching publication */
    web_DDP_Publication pub = web_DDP_getPublication(server, name);
    if (!pub) {
        web_DDP_Session_nosub(this, id);
    } else {
        web_DDP_Subscription sub =
            web_DDP_Session_getSub(this, pub, id, meta, value, scope);
        if (sub) {
            web_DDP_Publication_create(pub, this, sub);
        } else {
            web_DDP_Session_nosub(this, id);
        }
    }
/* $end */
}
