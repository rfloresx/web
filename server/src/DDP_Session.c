/* $CORTO_GENERATED
 *
 * DDP_Session.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */
#define SERVER_DDP_COLLECTION_REMOVE "{\"msg\":\"removed\",\"collection\":\"%s\"}"

corto_void server_DDP_Session_nosub(server_DDP_Session this, corto_string id) {
    server_HTTP server = server_Service(corto_parentof(corto_parentof(this)))->server;
    int msgLength = snprintf(NULL, 0, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);

    corto_string msg = corto_alloc(msgLength + 1);
    sprintf(msg, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);

    server_HTTP_write(server, this->conn, msg);
    corto_dealloc(msg);
}
/* $end */

corto_void _server_DDP_Session_connected(
    server_DDP_Session this)
{
/* $begin(corto/web/server/DDP/Session/connected) */
    int msgLength = snprintf(
        NULL, 0,
        "{\"msg\":\"connected\",\"session\":\"%s\"}",
        corto_nameof(this));

    corto_string msg = corto_alloc(msgLength + 1);
    sprintf(msg,
        "{\"msg\":\"connected\",\"session\":\"%s\"}", corto_nameof(this));

    server_SockJs_write(this->conn, msg);
    corto_dealloc(msg);
/* $end */
}

corto_int16 _server_DDP_Session_construct(
    server_DDP_Session this)
{
/* $begin(corto/web/server/DDP/Session/construct) */

    this->collections = corto_voidCreateChild(this, "__collections");
    this->subscriptions = corto_voidCreateChild(this, "__subscriptions");

    return 0;
/* $end */
}

corto_void _server_DDP_Session_error(
    server_DDP_Session this,
    corto_string reason,
    corto_string offendingMessage)
{
/* $begin(corto/web/server/DDP/Session/error) */
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
    server_SockJs_write(this->conn, msg);
/* $end */
}

corto_void _server_DDP_Session_failed(
    server_HTTP_Connection conn)
{
/* $begin(corto/web/server/DDP/Session/failed) */
    server_SockJs_write(conn,
        "{\"msg\":\"failed\",\"version\":\"1\"}");
/* $end */
}

server_DDP_Collection _server_DDP_Session_getCollection(
    server_DDP_Session this,
    corto_string name,
    corto_bool meta,
    corto_bool value,
    corto_bool scope)
{
/* $begin(corto/web/server/DDP/Session/getCollection) */
    corto_id collectionName;
    server_DDP_Collection result = NULL;
    corto_id subId;
    char *ptr = subId, ch;

    /* Replace / with _ */
    if (name) {
        strcpy(subId, name);

        while ((ch = *ptr)) {
            if (ch == '/') {
                *ptr = '_';
            }
            ptr++;
        }
    } else {
        subId[0] = '\0';
    }

    sprintf(collectionName, "c_%s", subId);

    if (!(result = corto_lookup(this->collections, collectionName))) {
        corto_object o = corto_resolve(NULL, name);
        if (o) {
            result = server_DDP_CollectionCreateChild(
                this->collections, collectionName, o, value, meta, scope);
        }
    }

    return result;
/* $end */
}

server_DDP_Subscription _server_DDP_Session_getSub(
    server_DDP_Session this,
    server_DDP_Publication pub,
    corto_string id,
    corto_bool meta,
    corto_bool value,
    corto_bool scope)
{
/* $begin(corto/web/server/DDP/Session/getSub) */
    server_DDP_Subscription result = NULL;

    if (!(result = corto_lookup(this->subscriptions, id))) {
        result = server_DDP_SubscriptionCreateChild(
            this->subscriptions, id, pub, id, value, meta, scope);
    }

    return result;
/* $end */
}

corto_void _server_DDP_Session_pong(
    server_DDP_Session this,
    corto_string id)
{
/* $begin(corto/web/server/DDP/Session/pong) */
    if (id) {
        int msgLength = snprintf(NULL, 0,
            "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        corto_string msg = corto_alloc(msgLength + 1);
        sprintf(msg, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        server_SockJs_write(this->conn, msg);
        corto_dealloc(msg);
    } else {
        server_SockJs_write(this->conn, "{\"msg\":\"pong\"}");
    }
/* $end */
}

corto_void _server_DDP_Session_sub(
    server_DDP_Session this,
    corto_string id,
    corto_string name,
    corto_bool meta,
    corto_bool value,
    corto_bool scope)
{
/* $begin(corto/web/server/DDP/Session/sub) */
    server_DDP server = server_DDP(corto_parentof(corto_parentof(this)));

    /* Find matching publication */
    server_DDP_Publication pub = server_DDP_getPublication(server, name);
    if (!pub) {
        server_DDP_Session_nosub(this, id);
    } else {
        server_DDP_Subscription sub =
            server_DDP_Session_getSub(this, pub, id, meta, value, scope);
        if (sub) {
            server_DDP_Publication_create(pub, this, sub);
        } else {
            server_DDP_Session_nosub(this, id);
        }
    }
/* $end */
}

corto_void _server_DDP_Session_unsub(
    server_DDP_Session this,
    corto_string id)
{
/* $begin(corto/web/server/DDP/Session/unsub) */
    server_DDP_Subscription sub = corto_lookup(this->subscriptions, id);
    if (sub) {
        corto_id collectionName;
        corto_string name = corto_nameof(sub->pub->scope);
        sprintf(collectionName, "c_%s", name ? name : "");
        server_DDP_Collection c = corto_lookup(
            this->collections, collectionName);
        if (c) {
            server_DDP_Collection_unsubscribe(c, sub->value, sub->meta, sub->scope);
            if (!(c->meta || c->value || c->scope)) {
                corto_string msg;
                corto_asprintf(&msg, SERVER_DDP_COLLECTION_REMOVE, name);
                server_SockJs_write(this->conn, msg);
                corto_dealloc(msg);
                corto_delete(c);
            }
        }
        corto_delete(sub);
    }
/* $end */
}
