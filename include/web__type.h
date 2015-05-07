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
#define web_server(o) ((web_server)cx_assertType((cx_type)web_server_o, o))
#define web_wsconnection(o) ((web_wsconnection)cx_assertType((cx_type)web_wsconnection_o, o))

/* Type definitions */
CX_LIST(cx_event_list);

/*  ::cortex::web::server */
CX_CLASS(web_server);

CX_CLASS_DEF(web_server) {
    cx_uint16 port;
    cx_uint32 connectionId;
    cx_word impl;
    cx_word thread;
    cx_event_list events;
    cx_bool defaultToConsole;
};

/*  ::cortex::web::wsconnection */
CX_CLASS(web_wsconnection);

CX_CLASS_DEF(web_wsconnection) {
    cx_object observing;
    cx_string remote_ip;
    cx_uint16 remote_port;
    cx_word conn;
    cx_uint32 eventCount;
};

#ifdef __cplusplus
}
#endif
#endif

