/* web_server.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef web_server_H
#define web_server_H

#include "cortex.h"
#include "web__type.h"

#include "web__api.h"

#include "web__meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ::cortex::web::server::construct() */
cx_int16 web_server_construct(web_server _this);

/* ::cortex::web::server::destruct() */
cx_void web_server_destruct(web_server _this);

/* ::cortex::web::server::post(event e) */
cx_void web_server_post(web_server _this, cx_event e);

#ifdef __cplusplus
}
#endif
#endif

