#ifndef web_DDPServer__method_H
#define web_DDPServer__method_H

#include "parson.h"

#include "cortex.h"
#include "json.h"
#include "web.h"

#ifdef __cplusplus
extern "C" {
#endif

cx_void web_DDPServer_methodInsert(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Value *json);
cx_void web_DDPServer_methodRemove(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Value* json);
cx_void web_DDPServer_methodUpdate(web_DDPServer _this, web_SockJsServer_Connection conn, JSON_Value* json);

#ifdef __cplusplus
}
#endif
#endif
