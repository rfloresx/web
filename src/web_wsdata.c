/* web_wsdata.c
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

 #define WEB_THROTTLE_THRESHOLD (200)

/* Write all parents to connection */
static void wsdata_sendParent(web_wsdata _this, cx_object o) {
    if (o) {
        web_wsdata_send(_this, o, FALSE, TRUE, FALSE, TRUE);
    }
} 
/* $end */

/* ::cortex::web::wsdata::construct() */
cx_int16 web_wsdata_construct(web_wsdata _this) {
/* $begin(::cortex::web::wsdata::construct) */
    CX_UNUSED(_this);

    web_wsdata_trigger_o->mask = CX_ON_DECLARE | CX_ON_DEFINE | CX_ON_UPDATE | CX_ON_SELF;

    return 0;
/* $end */
}

/* ::cortex::web::wsdata::destruct() */
cx_void web_wsdata_destruct(web_wsdata _this) {
/* $begin(::cortex::web::wsdata::destruct) */
    if (_this->observable) {
        cx_silence(_this->observable, 
            web_wsdata_trigger_o, _this);
    }
/* $end */
}

/* ::cortex::web::wsdata::send(object o,bool value,bool meta,bool scope,bool parents) */
cx_void web_wsdata_send(web_wsdata _this, cx_object o, cx_bool value, cx_bool meta, cx_bool scope, cx_bool parents) {
/* $begin(::cortex::web::wsdata::send) */
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);

    // If this is the first event after a subscription, also send data for parents */
    if (!_this->eventCount && parents) {
        wsdata_sendParent(_this, cx_parentof(o));
    }

    cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, meta, value, scope, TRUE};
    cx_serialize(&serializer, o, &jsonData);
    mg_websocket_printf((struct mg_connection *)_this->conn, WEBSOCKET_OPCODE_TEXT, 
        "{\"kind\":\"update\", \"observable\":[%s]}", jsonData.buffer);

    _this->serverSent++;

    cx_dealloc(jsonData.buffer);
/* $end */
}

/* ::cortex::web::wsdata::trigger() */
cx_void web_wsdata_trigger(web_wsdata _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::web::wsdata::trigger) */

    /* If the difference between received and send exceeds a threshold,
     * throttle down */
    if (_this->clientReceived && ((_this->serverSent - _this->clientReceived)) > WEB_THROTTLE_THRESHOLD) {
        cx_sleep(0, 500000000);
    }

    if (source && (observable != source)) {
        /* Only send metadata for objects not directly subscribed for */
        web_wsdata_send(_this, source, FALSE, TRUE, FALSE, FALSE);
    } else {
        web_wsdata_send(_this, observable, TRUE, TRUE, FALSE, TRUE);
        _this->eventCount++;
    }
/* $end */
}
