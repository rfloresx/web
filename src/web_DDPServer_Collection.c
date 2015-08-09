/* web_DDPServer_Collection.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */
#include "json.h"

#define WEB_DDPSERVER_COLLECTION_ADDED "{\"msg\":\"added\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define WEB_DDPSERVER_COLLECTION_CHANGED "{\"msg\":\"changed\",\"collection\":\"%s\",\"id\":\"%s\",\"fields\":%s}"
#define WEB_DDPSERVER_COLLECTION_REMOVED "{\"msg\":\"removed\",\"collection\":\"%s\",\"id\":\"%s\"}"

web_DDPServer_Session web_DDPServer_Collection_getSession(web_DDPServer_Collection _this) {
    return web_DDPServer_Session(cx_parentof(cx_parentof(_this)));
}
/* $end */

/* ::cortex::web::DDPServer::Collection::construct() */
cx_int16 web_DDPServer_Collection_construct(web_DDPServer_Collection _this) {
/* $begin(::cortex::web::DDPServer::Collection::construct) */
	web_DDPServer server = web_DDPServer(cx_parentof(cx_parentof(cx_parentof(cx_parentof(_this)))));

	cx_observer_setDispatcher(web_DDPServer_Collection_onAdded_o, server);
	cx_observer_setDispatcher(web_DDPServer_Collection_onChanged_o, server);
	cx_observer_setDispatcher(web_DDPServer_Collection_onRemoved_o, server);

    cx_listen(_this->obj, web_DDPServer_Collection_onAdded_o, _this);
    cx_listen(_this->obj, web_DDPServer_Collection_onChanged_o, _this);
    cx_listen(_this->obj, web_DDPServer_Collection_onRemoved_o, _this);

    return 0;
/* $end */
}

/* ::cortex::web::DDPServer::Collection::onAdded */
cx_void web_DDPServer_Collection_onAdded(web_DDPServer_Collection _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::DDPServer::Collection::onAdded) */
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    web_DDPServer_Session session = web_DDPServer_Collection_getSession(_this);
    cx_string msg = NULL;
    cx_uint32 msgLength = 0;
    cx_id id;

    CX_UNUSED(source);

    cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, _this->meta, _this->value, _this->scope, TRUE};
    cx_serialize(&serializer, observable, &jsonData);

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDPSERVER_COLLECTION_ADDED,
            cx_nameof(_this->obj), 
            cx_fullname(observable, id),
            jsonData.buffer);

    msg = cx_alloc(msgLength + 1);
    snprintf(msg, msgLength + 1, WEB_DDPSERVER_COLLECTION_ADDED, 
            cx_nameof(_this->obj), 
            id,
            jsonData.buffer);

    web_SockJsServer_Connection_send(session->conn, msg);

    cx_dealloc(msg);
/* $end */
}

/* ::cortex::web::DDPServer::Collection::onChanged */
cx_void web_DDPServer_Collection_onChanged(web_DDPServer_Collection _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::DDPServer::Collection::onChanged) */
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    web_DDPServer_Session session = web_DDPServer_Collection_getSession(_this);
    cx_string msg = NULL;
    cx_uint32 msgLength = 0;
    cx_id id;

    CX_UNUSED(source);

    cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, _this->meta, _this->value, _this->scope, TRUE};
    cx_serialize(&serializer, observable, &jsonData);

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDPSERVER_COLLECTION_CHANGED, 
            cx_nameof(_this->obj), 
            cx_fullname(observable, id),
            jsonData.buffer);

    msg = cx_alloc(msgLength + 1);
    snprintf(msg, msgLength + 1, WEB_DDPSERVER_COLLECTION_CHANGED, 
            cx_nameof(_this->obj), 
            id,
            jsonData.buffer);

    web_SockJsServer_Connection_send(session->conn, msg);

    cx_dealloc(msg);
/* $end */
}

/* ::cortex::web::DDPServer::Collection::onRemoved */
cx_void web_DDPServer_Collection_onRemoved(web_DDPServer_Collection _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::DDPServer::Collection::onRemoved) */
    web_DDPServer_Session session = web_DDPServer_Collection_getSession(_this);
    cx_string msg = NULL;
    cx_uint32 msgLength = 0;
    cx_id id;

    CX_UNUSED(source);

    /* Create message */
    msgLength = snprintf(NULL, 0, WEB_DDPSERVER_COLLECTION_REMOVED, 
            cx_nameof(_this->obj), 
            cx_fullname(observable, id));

    msg = cx_alloc(msgLength + 1);
    snprintf(msg, msgLength + 1, WEB_DDPSERVER_COLLECTION_REMOVED, 
            cx_nameof(_this->obj), 
            id);

    web_SockJsServer_Connection_send(session->conn, msg);

    cx_dealloc(msg);
/* $end */
}

/* ::cortex::web::DDPServer::Collection::subscribe(bool value,bool meta,bool scope) */
cx_void web_DDPServer_Collection_subscribe(web_DDPServer_Collection _this, cx_bool value, cx_bool meta, cx_bool scope) {
/* $begin(::cortex::web::DDPServer::Collection::subscribe) */

    _this->value |= value;
    _this->meta |= meta;
    _this->scope |= scope;

/* $end */
}
