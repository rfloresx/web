/* web_DDPServer_Collection.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#define corto_web_LIB
#include "web.h"

/* $header() */
#include "json.h"

#define WEB_DDPSERVER_COLLECTION_ADDED "{\"msg\":\"added\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define WEB_DDPSERVER_COLLECTION_CHANGED "{\"msg\":\"changed\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define WEB_DDPSERVER_COLLECTION_REMOVED "{\"msg\":\"removed\",\"collection\":\"%s\",\"id\":\"%s\"}"

web_DDPServer_Session web_DDPServer_Collection_getSession(web_DDPServer_Collection this) {
    return web_DDPServer_Session(cx_parentof(cx_parentof(this)));
}
/* $end */

/* ::corto::web::DDPServer::Collection::construct() */
cx_int16 _web_DDPServer_Collection_construct(web_DDPServer_Collection this) {
/* $begin(::corto::web::DDPServer::Collection::construct) */
	web_DDPServer server = web_DDPServer(cx_parentof(cx_parentof(cx_parentof(cx_parentof(this)))));

	cx_observer_setDispatcher(web_DDPServer_Collection_onAdded_o, server);
	cx_observer_setDispatcher(web_DDPServer_Collection_onChanged_o, server);
	cx_observer_setDispatcher(web_DDPServer_Collection_onRemoved_o, server);

    cx_listen(this->obj, web_DDPServer_Collection_onAdded_o, this);
    cx_listen(this->obj, web_DDPServer_Collection_onChanged_o, this);
    cx_listen(this->obj, web_DDPServer_Collection_onRemoved_o, this);

    return 0;
/* $end */
}

/* ::corto::web::DDPServer::Collection::onAdded */
cx_void _web_DDPServer_Collection_onAdded(web_DDPServer_Collection this, cx_object observable, cx_object source) {
/* $begin(::corto::web::DDPServer::Collection::onAdded) */
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    web_DDPServer_Session session = web_DDPServer_Collection_getSession(this);
    cx_string msg = NULL;
    cx_uint32 msgLength = 0;
    cx_id id;

    CX_UNUSED(source);

    cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, this->meta, this->value, this->scope, TRUE, FALSE};
    cx_serialize(&serializer, observable, &jsonData);

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDPSERVER_COLLECTION_ADDED,
            cx_nameof(this->obj),
            cx_fullname(observable, id),
            jsonData.buffer);

    msg = cx_alloc(msgLength + 1);

    snprintf(msg, msgLength + 1, WEB_DDPSERVER_COLLECTION_ADDED,
            cx_nameof(this->obj),
            id,
            jsonData.buffer);

    web_SockJsServer_Connection_send(session->conn, msg);

    cx_dealloc(msg);
/* $end */
}

/* ::corto::web::DDPServer::Collection::onChanged */
cx_void _web_DDPServer_Collection_onChanged(web_DDPServer_Collection this, cx_object observable, cx_object source) {
/* $begin(::corto::web::DDPServer::Collection::onChanged) */
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    web_DDPServer_Session session = web_DDPServer_Collection_getSession(this);
    cx_string msg = NULL;
    cx_uint32 msgLength = 0;
    cx_id id;

    CX_UNUSED(source);

    cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, this->meta, this->value, this->scope, TRUE, FALSE};
    cx_serialize(&serializer, observable, &jsonData);

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDPSERVER_COLLECTION_CHANGED,
            cx_nameof(this->obj),
            cx_fullname(observable, id),
            jsonData.buffer);

    msg = cx_alloc(msgLength + 1);

    snprintf(msg, msgLength + 1, WEB_DDPSERVER_COLLECTION_CHANGED,
            cx_nameof(this->obj),
            id,
            jsonData.buffer);

    web_SockJsServer_Connection_send(session->conn, msg);

    cx_dealloc(msg);
/* $end */
}

/* ::corto::web::DDPServer::Collection::onRemoved */
cx_void _web_DDPServer_Collection_onRemoved(web_DDPServer_Collection this, cx_object observable, cx_object source) {
/* $begin(::corto::web::DDPServer::Collection::onRemoved) */
    web_DDPServer_Session session = web_DDPServer_Collection_getSession(this);
    cx_string msg = NULL;
    cx_uint32 msgLength = 0;
    cx_id id;

    CX_UNUSED(source);

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDPSERVER_COLLECTION_REMOVED,
            cx_nameof(this->obj),
            cx_fullname(observable, id));

    msg = cx_alloc(msgLength + 1);

    snprintf(msg, msgLength + 1, WEB_DDPSERVER_COLLECTION_REMOVED,
            cx_nameof(this->obj),
            id);

    web_SockJsServer_Connection_send(session->conn, msg);

    cx_dealloc(msg);
/* $end */
}

/* ::corto::web::DDPServer::Collection::subscribe(bool value,bool meta,bool scope) */
cx_void _web_DDPServer_Collection_subscribe(web_DDPServer_Collection this, cx_bool value, cx_bool meta, cx_bool scope) {
/* $begin(::corto::web::DDPServer::Collection::subscribe) */

    this->value |= value;
    this->meta |= meta;
    this->scope |= scope;

/* $end */
}
