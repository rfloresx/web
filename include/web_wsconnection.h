/* web_wsconnection.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef web_wsconnection_H
#define web_wsconnection_H

#include "cortex.h"
#include "web__type.h"

#include "web__api.h"

#include "web__meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ::cortex::web::wsconnection::construct() */
cx_int16 web_wsconnection_construct(web_wsconnection _this);

/* ::cortex::web::wsconnection::send(object o,bool value,bool meta,bool scope,bool parents) */
cx_void web_wsconnection_send(web_wsconnection _this, cx_object o, cx_bool value, cx_bool meta, cx_bool scope, cx_bool parents);

/* ::cortex::web::wsconnection::trigger() */
cx_void web_wsconnection_trigger(web_wsconnection _this, cx_object *observable, cx_object *source);

#ifdef __cplusplus
}
#endif
#endif

