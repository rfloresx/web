/* server.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_WEB_SERVER_H
#define CORTO_WEB_SERVER_H

#include "corto/corto.h"
#include "corto/web/server/_type.h"
#include "corto/web/server/_api.h"
#include "corto/web/server/_meta.h"
#include "corto/web/server/_interface.h"

#ifdef __cplusplus
extern "C" {
#endif


CORTO_WEB_SERVER_EXPORT server_UrlMatch _server_match(
    corto_string pattern,
    corto_string url);
#define server_match(pattern, url) _server_match(pattern, url)

CORTO_WEB_SERVER_EXPORT corto_rbtree _server_queryToMap(
    corto_string query);
#define server_queryToMap(query) _server_queryToMap(query)

CORTO_WEB_SERVER_EXPORT corto_string _server_random(
    corto_uint16 n);
#define server_random(n) _server_random(n)

CORTO_WEB_SERVER_EXPORT corto_string _server_urlDecode(
    corto_string s);
#define server_urlDecode(s) _server_urlDecode(s)
#include "corto/web/server/DDP.h"
#include "corto/web/server/DDP_Collection.h"
#include "corto/web/server/DDP_Publication.h"
#include "corto/web/server/DDP_readyEvent.h"
#include "corto/web/server/DDP_Session.h"
#include "corto/web/server/DDP_Subscription.h"
#include "corto/web/server/Files.h"
#include "corto/web/server/HTTP.h"
#include "corto/web/server/HTTP_Connection.h"
#include "corto/web/server/HTTP_Request.h"
#include "corto/web/server/REST.h"
#include "corto/web/server/Route.h"
#include "corto/web/server/Router.h"
#include "corto/web/server/RouteService.h"
#include "corto/web/server/Service.h"
#include "corto/web/server/SockJs.h"
#include "corto/web/server/StandaloneHTTP.h"
#include "corto/web/server/UrlMatch.h"
#include "corto/web/web.h"
#include "corto/fmt/json/json.h"

#ifdef __cplusplus
}
#endif
#endif

