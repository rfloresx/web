/* rest__type.h
 *
 *    Type-definitions for C-language.
 *    This file contains generated code. Do not modify!
 */

#ifndef rest__type_H
#define rest__type_H

#include "cortex.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Casting macro's for classes */
#define rest_server(o) ((rest_server)o)

/* Type definitions */
/*  ::cortex::rest::server */
CX_CLASS(rest_server);

CX_CLASS_DEF(rest_server) {
    cx_uint16 port;
    cx_word impl;
    cx_word thread;
};

#ifdef __cplusplus
}
#endif
#endif

