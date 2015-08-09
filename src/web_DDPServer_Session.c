/* web_DDPServer_Session.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */
cx_void web_DDPServer_Session_nosub(web_DDPServer_Session _this, cx_string id) {
    int msgLength = snprintf(NULL, 0, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);
    cx_string msg = cx_alloc(msgLength + 1);
    sprintf(msg, "{\"msg\":\"nosub\",\"id\":\"%s\"}", id);
    web_SockJsServer_Connection_send(_this->conn, msg);
    cx_dealloc(msg);
}
/* $end */

/* ::cortex::web::DDPServer::Session::connected() */
cx_void web_DDPServer_Session_connected(web_DDPServer_Session _this) {
/* $begin(::cortex::web::DDPServer::Session::connected) */
    int msgLength = snprintf(NULL, 0, "{\"msg\":\"connected\",\"session\":\"%s\"}", cx_nameof(_this));
    cx_string msg = cx_alloc(msgLength + 1);
    sprintf(msg, "{\"msg\":\"connected\",\"session\":\"%s\"}", cx_nameof(_this));
    web_SockJsServer_Connection_send(_this->conn, msg);
    cx_dealloc(msg);
/* $end */
}

/* ::cortex::web::DDPServer::Session::construct() */
cx_int16 web_DDPServer_Session_construct(web_DDPServer_Session _this) {
/* $begin(::cortex::web::DDPServer::Session::construct) */

    _this->collections = cx_void__createChild(_this, "__collections");
    _this->subscriptions = cx_void__createChild(_this, "__subscriptions");

    return 0;
/* $end */
}

/* ::cortex::web::DDPServer::Session::error(string reason,string offendingMessage) */
cx_void web_DDPServer_Session_error(web_DDPServer_Session _this, cx_string reason, cx_string offendingMessage) {
/* $begin(::cortex::web::DDPServer::Session::error) */
    cx_string msg = NULL;
    if (offendingMessage) {
        cx_asprintf(&msg, "{\"msg\":\"error\",\"reason\":\"%s\",\"offendingMessage\":\"%s\"}", reason, offendingMessage);
    } else {
        cx_asprintf(&msg, "{\"msg\":\"error\",\"reason\":\"%s\"}", reason);
    }
    cx_dealloc(msg);
    web_SockJsServer_Connection_send(_this->conn, msg);
/* $end */
}

/* ::cortex::web::DDPServer::Session::failed(::cortex::web::SockJsServer::Connection conn) */
cx_void web_DDPServer_Session_failed(web_SockJsServer_Connection conn) {
/* $begin(::cortex::web::DDPServer::Session::failed) */
    web_SockJsServer_Connection_send(conn, "{\"msg\":\"failed\",\"version\":\"1\"}");
/* $end */
}

/* ::cortex::web::DDPServer::Session::getCollection(string name) */
web_DDPServer_Collection web_DDPServer_Session_getCollection(web_DDPServer_Session _this, cx_string name) {
/* $begin(::cortex::web::DDPServer::Session::getCollection) */
    web_DDPServer_Collection result = NULL;

    if (!(result = cx_lookup(_this->collections, name))) {
        cx_object o = cx_resolve(NULL, name);
        if (o) {
            result = web_DDPServer_Collection__createChild(_this->collections, name, o, FALSE, FALSE, FALSE);
        }
    }

    return result;
/* $end */
}

/* ::cortex::web::DDPServer::Session::getSub(::cortex::web::DDPServer::Publication pub,string id,bool meta,bool value,bool scope) */
web_DDPServer_Subscription web_DDPServer_Session_getSub(web_DDPServer_Session _this, web_DDPServer_Publication pub, cx_string id, cx_bool meta, cx_bool value, cx_bool scope) {
/* $begin(::cortex::web::DDPServer::Session::getSub) */
    web_DDPServer_Subscription result = NULL;

    if (!(result = cx_lookup(_this->subscriptions, id))) {
        result = web_DDPServer_Subscription__createChild(_this->subscriptions, id, pub, id, value, meta, scope);
    }

    return result;
/* $end */
}

/* ::cortex::web::DDPServer::Session::pong(string id) */
cx_void web_DDPServer_Session_pong(web_DDPServer_Session _this, cx_string id) {
/* $begin(::cortex::web::DDPServer::Session::pong) */
    if (id) {
        int msgLength = snprintf(NULL, 0, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        cx_string msg = cx_alloc(msgLength + 1);
        sprintf(msg, "{\"msg\":\"pong\",\"id\":\"%s\"}", id);
        web_SockJsServer_Connection_send(_this->conn, msg);
        cx_dealloc(msg);
    } else {
        web_SockJsServer_Connection_send(_this->conn, "{\"msg\":\"pong\"}");
    }
/* $end */
}

/* ::cortex::web::DDPServer::Session::sub(string id,string name,bool meta,bool value,bool scope) */
cx_void web_DDPServer_Session_sub(web_DDPServer_Session _this, cx_string id, cx_string name, cx_bool meta, cx_bool value, cx_bool scope) {
/* $begin(::cortex::web::DDPServer::Session::sub) */
    web_DDPServer server = web_DDPServer(cx_parentof(cx_parentof(_this)));

    /* Find matching publication */
    web_DDPServer_Publication pub = web_DDPServer_getPublication(server, name);
    if (!pub) {
        web_DDPServer_Session_nosub(_this, id);
    } else {
        web_DDPServer_Subscription sub = web_DDPServer_Session_getSub(_this, pub, id, meta, value, scope);
        if (sub) {
            web_DDPServer_Publication_create(pub, _this, sub);
        } else {
            web_DDPServer_Session_nosub(_this, id);
        }
    }
/* $end */
}
