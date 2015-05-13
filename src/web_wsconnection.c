/* web_wsconnection.c
 *
 * This file contains the implementation for the generated interface.
 *
 *    Don't mess with the begin and end tags, since these will ensure that modified
 *    code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"
#include "web__meta.h"

/* $header() */
#include "mongoose.h"
#include "json.h"

#define WEB_THROTTLE_THRESHOLD (1000)

/* Write all parents to connection */
static void wsdata_sendParent(web_wsconnection _this, cx_object o) {
    if (o) {
        web_wsconnection_send(_this, o, FALSE, TRUE, FALSE, TRUE, FALSE);
    }
} 
/* $end */

/* ::cortex::web::wsconnection::construct() */
cx_int16 web_wsconnection_construct(web_wsconnection _this) {
/* $begin(::cortex::web::wsconnection::construct) */
    CX_UNUSED(_this);
    web_wsconnection_onUpdate_o->mask = CX_ON_DECLARE | CX_ON_DEFINE | CX_ON_UPDATE | CX_ON_SELF;
    web_wsconnection_onDelete_o->mask = CX_ON_DELETE | CX_ON_SELF | CX_ON_SCOPE;
    return 0;
/* $end */
}

/* ::cortex::web::wsconnection::onDelete() */
cx_void web_wsconnection_onDelete(web_wsconnection _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::wsconnection::onDelete) */
    CX_UNUSED(source);
    if (_this->conn) {
        web_wsconnection_send(_this, observable, FALSE, TRUE, FALSE, FALSE, TRUE);
    }
/* $end */
}

/* ::cortex::web::wsconnection::onUpdate() */
cx_void web_wsconnection_onUpdate(web_wsconnection _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::wsconnection::onUpdate) */
    if (_this->conn) {
        if (source && (observable != source)) {
            /* Only send metadata for objects not directly subscribed for */
            web_wsconnection_send(_this, source, FALSE, TRUE, FALSE, FALSE, FALSE);
        } else {
            web_wsconnection_send(_this, observable, TRUE, TRUE, FALSE, TRUE, FALSE);
            _this->eventCount++;
        }
    }
/* $end */
}

/* ::cortex::web::wsconnection::send(object o,bool value,bool meta,bool scope,bool parents,bool isDelete) */
cx_void web_wsconnection_send(web_wsconnection _this, cx_object o, cx_bool value, cx_bool meta, cx_bool scope, cx_bool parents, cx_bool isDelete) {
/* $begin(::cortex::web::wsconnection::send) */
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);

    // If this is the first event after a subscription, also send data for parents */
    if (!_this->eventCount && parents) {
        wsdata_sendParent(_this, cx_parentof(o));
    }

    cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, meta, value, scope, TRUE};
    cx_serialize(&serializer, o, &jsonData);

    if (!isDelete) {
        mg_websocket_printf((struct mg_connection *)_this->conn, WEBSOCKET_OPCODE_TEXT, 
            "{\"t\":\"u\", \"o\":%s}", jsonData.buffer);
    } else {
        mg_websocket_printf((struct mg_connection *)_this->conn, WEBSOCKET_OPCODE_TEXT, 
            "{\"t\":\"d\", \"o\":%s}", jsonData.buffer);        
    }

    cx_dealloc(jsonData.buffer);
/* $end */
}
