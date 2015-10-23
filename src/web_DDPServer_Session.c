/* web_DDPServer_Session.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */
corto_void web_DDPServer_Session_nosub(web_DDPServer_Session this, corto_string id) {
    int msgLength = snprintf(NULL, 0, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);
    corto_string msg = corto_alloc(msgLength + 1);
    sprintf(msg, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);
    web_SockJsServer_Connection_send(this->conn, msg);
    corto_dealloc(msg);
}
/* $end */

/* ::corto::web::DDPServer::Session::connected() */
corto_void _web_DDPServer_Session_connected(web_DDPServer_Session this) {
/* $begin(::corto::web::DDPServer::Session::connected) */
    int msgLength = snprintf(NULL, 0, "{\"msg\":\"connected\",\"session\":\"%s\"}", corto_nameof(this));
    corto_string msg = corto_alloc(msgLength + 1);
    sprintf(msg, "{\"msg\":\"connected\",\"session\":\"%s\"}", corto_nameof(this));
    web_SockJsServer_Connection_send(this->conn, msg);
    corto_dealloc(msg);
/* $end */
}

/* ::corto::web::DDPServer::Session::construct() */
corto_int16 _web_DDPServer_Session_construct(web_DDPServer_Session this) {
/* $begin(::corto::web::DDPServer::Session::construct) */

    this->collections = corto_voidCreateChild(this, "__collections");
    this->subscriptions = corto_voidCreateChild(this, "__subscriptions");

    return 0;
/* $end */
}

/* ::corto::web::DDPServer::Session::error(string reason,string offendingMessage) */
corto_void _web_DDPServer_Session_error(web_DDPServer_Session this, corto_string reason, corto_string offendingMessage) {
/* $begin(::corto::web::DDPServer::Session::error) */
    corto_string msg = NULL;
    if (offendingMessage) {
        corto_asprintf(&msg, "{\"msg\":\"error\",\"reason\":\"%s\",\"offendingMessage\":\"%s\"}", reason, offendingMessage);
    } else {
        corto_asprintf(&msg, "{\"msg\":\"error\",\"reason\":\"%s\"}", reason);
    }
    corto_dealloc(msg);
    web_SockJsServer_Connection_send(this->conn, msg);
/* $end */
}

/* ::corto::web::DDPServer::Session::failed(::corto::web::SockJsServer::Connection conn) */
corto_void _web_DDPServer_Session_failed(web_SockJsServer_Connection conn) {
/* $begin(::corto::web::DDPServer::Session::failed) */
    web_SockJsServer_Connection_send(conn, "{\"msg\":\"failed\",\"version\":\"1\"}");
/* $end */
}

/* ::corto::web::DDPServer::Session::getCollection(string name) */
web_DDPServer_Collection _web_DDPServer_Session_getCollection(web_DDPServer_Session this, corto_string name) {
/* $begin(::corto::web::DDPServer::Session::getCollection) */
    web_DDPServer_Collection result = NULL;

    if (!(result = corto_lookup(this->collections, name))) {
        corto_object o = corto_resolve(NULL, name);
        if (o) {
            result = web_DDPServer_CollectionCreateChild(this->collections, name, o, FALSE, FALSE, FALSE);
        }
    }

    return result;
/* $end */
}

/* ::corto::web::DDPServer::Session::getSub(::corto::web::DDPServer::Publication pub,string id,bool meta,bool value,bool scope) */
web_DDPServer_Subscription _web_DDPServer_Session_getSub(web_DDPServer_Session this, web_DDPServer_Publication pub, corto_string id, corto_bool meta, corto_bool value, corto_bool scope) {
/* $begin(::corto::web::DDPServer::Session::getSub) */
    web_DDPServer_Subscription result = NULL;

    if (!(result = corto_lookup(this->subscriptions, id))) {
        result = web_DDPServer_SubscriptionCreateChild(this->subscriptions, id, pub, id, value, meta, scope);
    }

    return result;
/* $end */
}

/* ::corto::web::DDPServer::Session::pong(string id) */
corto_void _web_DDPServer_Session_pong(web_DDPServer_Session this, corto_string id) {
/* $begin(::corto::web::DDPServer::Session::pong) */
    if (id) {
        int msgLength = snprintf(NULL, 0, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        corto_string msg = corto_alloc(msgLength + 1);
        sprintf(msg, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        web_SockJsServer_Connection_send(this->conn, msg);
        corto_dealloc(msg);
    } else {
        web_SockJsServer_Connection_send(this->conn, "{\"msg\":\"pong\"}");
    }
/* $end */
}

/* ::corto::web::DDPServer::Session::sub(string id,string name,bool meta,bool value,bool scope) */
corto_void _web_DDPServer_Session_sub(web_DDPServer_Session this, corto_string id, corto_string name, corto_bool meta, corto_bool value, corto_bool scope) {
/* $begin(::corto::web::DDPServer::Session::sub) */
    web_DDPServer server = web_DDPServer(corto_parentof(corto_parentof(this)));

    /* Find matching publication */
    web_DDPServer_Publication pub = web_DDPServer_getPublication(server, name);
    if (!pub) {
        web_DDPServer_Session_nosub(this, id);
    } else {
        web_DDPServer_Subscription sub = web_DDPServer_Session_getSub(this, pub, id, meta, value, scope);
        if (sub) {
            web_DDPServer_Publication_create(pub, this, sub);
        } else {
            web_DDPServer_Session_nosub(this, id);
        }
    }
/* $end */
}
