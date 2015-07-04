/* web_DDPConnection.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */

#include "json.h"
#include "parson.h"

/* $end */

/* ::cortex::web::DDPConnection::connected() */
cx_void web_DDPConnection_connected(web_DDPConnection _this) {
/* $begin(::cortex::web::DDPConnection::connected) */
    web_WebSocketConnection __this = web_WebSocketConnection(_this);
    JSON_Value *value = json_parse_string("{\"msg\": \"connected\"}");
    JSON_Object *obj = json_value_get_object(value);
    char *serialized_string = NULL;

    json_object_set_string(obj, "session", cx_nameof(_this));
    serialized_string = json_serialize_to_string(value);
    web_WebSocketConnection_send(__this, serialized_string);

    json_free_serialized_string(serialized_string);
    json_value_free(value);
/* $end */
}

/* ::cortex::web::DDPConnection::destruct() */
cx_void web_DDPConnection_destruct(web_DDPConnection _this) {
/* $begin(::cortex::web::DDPConnection::destruct) */
    if (_this->expectingPongId) {
        cx_dealloc(_this->expectingPongId);
        _this->expectingPongId = NULL;
    }
/* $end */
}

/* ::cortex::web::DDPConnection::failed() */
cx_void web_DDPConnection_failed(web_DDPConnection _this) {
/* $begin(::cortex::web::DDPConnection::failed) */
    web_WebSocketConnection __this = web_WebSocketConnection(_this);
    web_WebSocketConnection_send(__this, "{\"msg\": \"failed\", \"version\": \"1\"}");
    CX_UNUSED(_this);
/* $end */
}

/* ::cortex::web::DDPConnection::nosub() */
cx_void web_DDPConnection_nosub(web_DDPConnection _this) {
/* $begin(::cortex::web::DDPConnection::nosub) */
    CX_UNUSED(_this);
/* $end */
}

/* ::cortex::web::DDPConnection::onDelete() */
cx_void web_DDPConnection_onDelete(web_DDPConnection _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::DDPConnection::onDelete) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    CX_UNUSED(source);
/* $end */
}

/* ::cortex::web::DDPConnection::onUpdate() */
cx_void web_DDPConnection_onUpdate(web_DDPConnection _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::DDPConnection::onUpdate) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    CX_UNUSED(source);
/* $end */
}

/* ::cortex::web::DDPConnection::ping(string id) */
cx_void web_DDPConnection_ping(web_DDPConnection _this, cx_string id) {
/* $begin(::cortex::web::DDPConnection::ping) */
    web_WebSocketConnection __this = web_WebSocketConnection(_this);
    JSON_Value *value = json_parse_string("{\"msg\": \"ping\"}");
    JSON_Object *obj = json_value_get_object(value);
    char *serialized_string;
    _this->expectingPong = TRUE;
    if (_this->expectingPongId) {
        cx_dealloc(_this->expectingPongId);
        _this->expectingPongId = NULL;
    }
    if (id) {
        json_object_set_string(obj, "id", id);
        _this->expectingPongId = cx_malloc(strlen(id) + 1);
        strcpy(_this->expectingPongId, id);
    }
    serialized_string = json_serialize_to_string(value);
    web_WebSocketConnection_send(__this, serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(value);
/* $end */
}

/* ::cortex::web::DDPConnection::pong(string id) */
cx_void web_DDPConnection_pong(web_DDPConnection _this, cx_string id) {
/* $begin(::cortex::web::DDPConnection::pong) */
    web_WebSocketConnection __this = web_WebSocketConnection(_this);
    JSON_Value *value = json_parse_string("{\"msg\": \"pong\"}");
    JSON_Object *obj = json_value_get_object(value);
    char *serialized_string;
    if (id) {
        json_object_set_string(obj, "id", id);
    }
    serialized_string = json_serialize_to_string(value);
    web_WebSocketConnection_send(__this, serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(value);
    web_DDPConnection_ping(_this, "hello");
/* $end */
}

/* ::cortex::web::DDPConnection::send(object o,bool value,bool meta,bool scope,bool parents,bool isDelete) */
/* $header(::cortex::web::DDPConnection::send) */
static void wsdata_sendParent(web_DDPConnection _this, cx_object o) {
    if (o) {
        web_DDPConnection_send(_this, o, FALSE, TRUE, FALSE, TRUE, FALSE);
    }
}
/* $end */
cx_void web_DDPConnection_send(web_DDPConnection _this, cx_object o, cx_bool value, cx_bool meta, cx_bool scope, cx_bool parents, cx_bool isDelete) {
/* $begin(::cortex::web::DDPConnection::send) */
    CX_UNUSED(_this);
    CX_UNUSED(o);
    CX_UNUSED(value);
    CX_UNUSED(meta);
    CX_UNUSED(scope);
    CX_UNUSED(parents);
    CX_UNUSED(isDelete);

    // web_WebSocketConnection __this = web_WebSocketConnection(_this); // TODO too much overhead?
    // struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);

    // // If this is the first event after a subscription, also send data for parents */
    // if (!_this->eventCount && parents) {
    //     wsdata_sendParent(_this, cx_parentof(o));
    // }

    // cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, meta, value, scope, TRUE};
    // cx_serialize(&serializer, o, &jsonData);

    // if (!isDelete) {
    //     mg_websocket_printf((struct mg_connection *)__this->conn, WEBSOCKET_OPCODE_TEXT, 
    //         "{\"t\":\"u\", \"o\":%s}", jsonData.buffer);
    // } else {
    //     mg_websocket_printf((struct mg_connection *)__this->conn, WEBSOCKET_OPCODE_TEXT, 
    //         "{\"t\":\"d\", \"o\":%s}", jsonData.buffer);        
    // }

    // cx_dealloc(jsonData.buffer);
/* $end */
}

