/* web__type.h
 *
 *    Type-definitions for C-language.
 *    This file contains generated code. Do not modify!
 */

#ifndef web__type_H
#define web__type_H

#include "cortex.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Casting macro's for classes */
#define web_server(o) ((web_server)o)
#define web_wsdata(o) ((web_wsdata)o)

/* Type definitions */
/*  ::cortex::web::server */
CX_CLASS(web_server);

CX_CLASS_DEF(web_server) {
    cx_uint16 port;
    cx_word impl;
    cx_word thread;
};

/*  ::cortex::web::wsdata */
CX_CLASS(web_wsdata);

CX_CLASS_DEF(web_wsdata) {
    cx_word conn;
    cx_object observable;
    cx_uint32 eventCount;
    cx_uint32 clientReceived;
    cx_uint32 serverSent;
};

#ifdef __cplusplus
}
#endif
#endif

