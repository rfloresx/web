/* web_wsdata.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef web_wsdata_H
#define web_wsdata_H

#include "cortex.h"
#include "web__type.h"

#include "web__api.h"

#include "web__meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ::cortex::web::wsdata::construct() */
cx_int16 web_wsdata_construct(web_wsdata _this);

/* ::cortex::web::wsdata::destruct() */
cx_void web_wsdata_destruct(web_wsdata _this);

/* ::cortex::web::wsdata::send(object o,bool value,bool meta,bool scope,bool parents) */
cx_void web_wsdata_send(web_wsdata _this, cx_object o, cx_bool value, cx_bool meta, cx_bool scope, cx_bool parents);

/* ::cortex::web::wsdata::trigger() */
cx_void web_wsdata_trigger(web_wsdata _this, cx_object *observable, cx_object *source);

#ifdef __cplusplus
}
#endif
#endif

