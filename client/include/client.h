/* client.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_WEB_CLIENT_H
#define CORTO_WEB_CLIENT_H

#include <corto/corto.h>
#include <corto/web/client/_project.h>
#include <corto/web/web.h>
#include <corto/core/c/c.h>
#include <corto/lang/c/c.h>

#include <corto/web/client/_type.h>
#include <corto/web/client/_api.h>
#include <corto/web/client/_load.h>

#ifdef __cplusplus
extern "C" {
#endif


CORTO_WEB_CLIENT_EXPORT web_client_Result _web_client_get(
    corto_string url,
    corto_string fields);
#define web_client_get(url, fields) _web_client_get(url, fields)

CORTO_WEB_CLIENT_EXPORT web_client_Result _web_client_post(
    corto_string url,
    corto_string fields);
#define web_client_post(url, fields) _web_client_post(url, fields)
#include <corto/web/client/Result.h>
#include <corto/web/client/Url.h>

#ifdef __cplusplus
}
#endif
#endif

