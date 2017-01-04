/* server.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_WEB_SERVER_H
#define CORTO_WEB_SERVER_H

#include <corto/corto.h>
#include <corto/web/server/_project.h>
#include <evhtp/evhtp.h>
#include <event2/event2.h>
#include <corto/fmt/json/json.h>
#include <corto/web/web.h>
#include <corto/core/c/c.h>
#include <corto/lang/c/c.h>

#include <corto/web/server/_type.h>

#include <corto/web/server/_api.h>
#include <corto/web/server/_load.h>
#ifdef __cplusplus
extern "C" {
#endif


CORTO_WEB_SERVER_EXPORT corto_string _server_random(
    corto_uint16 n);
#define server_random(n) _server_random(n)

CORTO_WEB_SERVER_EXPORT corto_string _server_typedescriptor(
    corto_type type);
#define server_typedescriptor(type) _server_typedescriptor(corto_type(type))
#include <corto/web/server/DDP.h>
#include <corto/web/server/DDP_Collection.h>
#include <corto/web/server/DDP_Publication.h>
#include <corto/web/server/DDP_readyEvent.h>
#include <corto/web/server/DDP_Session.h>
#include <corto/web/server/DDP_Subscription.h>
#include <corto/web/server/DELETE.h>
#include <corto/web/server/Files.h>
#include <corto/web/server/GET.h>
#include <corto/web/server/HTTP.h>
#include <corto/web/server/HTTP_Connection.h>
#include <corto/web/server/HTTP_Request.h>
#include <corto/web/server/POST.h>
#include <corto/web/server/PUT.h>
#include <corto/web/server/REST.h>
#include <corto/web/server/Router.h>
#include <corto/web/server/RouterService.h>
#include <corto/web/server/Service.h>
#include <corto/web/server/SockJs.h>
#include <corto/web/server/StandaloneHTTP.h>
#include <corto/web/server/StandaloneHTTPS.h>

#ifdef __cplusplus
}
#endif
#endif

