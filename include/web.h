/* web.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_WEB_H
#define CORTO_WEB_H

#include <corto/corto.h>
#include <corto/corto.h>
#include <corto/web/_project.h>
#include <corto/core/c/c.h>
#include <corto/lang/c/c.h>

/* $header() */
/* Enter additional code here. */
/* $end */

#include <corto/web/_type.h>
#include <corto/web/_load.h>
#include <corto/web/_api.h>

/* $body() */
/* Enter code that requires types here */
/* $end */

#ifdef __cplusplus
extern "C" {
#endif

#define web_escapeFromRequest(data) _web_escapeFromRequest(data)
CORTO_WEB_EXPORT
corto_string _web_escapeFromRequest(
    corto_string data);

#define web_escapeToRequest(url) _web_escapeToRequest(url)
CORTO_WEB_EXPORT
corto_string _web_escapeToRequest(
    corto_string url);


#ifdef __cplusplus
}
#endif

#endif

