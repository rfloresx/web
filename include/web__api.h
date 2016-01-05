/* web__api.h
 *
 * API convenience functions for C-language.
 * This file contains generated code. Do not modify!
 */

#ifndef corto_web__API_H
#define corto_web__API_H

#include "corto.h"
#include "web__interface.h"
#ifdef __cplusplus
extern "C" {
#endif
/* /corto/web/DDPServer */
CORTO_WEB_EXPORT web_DDPServer _web_DDPServerCreate(corto_uint16 port);
#define web_DDPServerCreate(port) _web_DDPServerCreate(port)
CORTO_WEB_EXPORT web_DDPServer _web_DDPServerCreateChild(corto_object _parent, corto_string _name, corto_uint16 port);
#define web_DDPServerCreateChild(_parent, _name, port) _web_DDPServerCreateChild(_parent, _name, port)
CORTO_WEB_EXPORT corto_int16 _web_DDPServerUpdate(web_DDPServer _this, corto_uint16 port);
#define web_DDPServerUpdate(_this, port) _web_DDPServerUpdate(web_DDPServer(_this), port)

CORTO_WEB_EXPORT web_DDPServer _web_DDPServerDeclare(void);
#define web_DDPServerDeclare() _web_DDPServerDeclare()
CORTO_WEB_EXPORT web_DDPServer _web_DDPServerDeclareChild(corto_object _parent, corto_string _name);
#define web_DDPServerDeclareChild(_parent, _name) _web_DDPServerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_DDPServerDefine(web_DDPServer _this, corto_uint16 port);
#define web_DDPServerDefine(_this, port) _web_DDPServerDefine(web_DDPServer(_this), port)
CORTO_WEB_EXPORT void _web_DDPServerSet(web_DDPServer _this, corto_uint16 port);
#define web_DDPServerSet(_this, port) _web_DDPServerSet(web_DDPServer(_this), port)
CORTO_WEB_EXPORT corto_string _web_DDPServerStr(web_DDPServer value);
#define web_DDPServerStr(value) _web_DDPServerStr(web_DDPServer(value))
CORTO_WEB_EXPORT web_DDPServer web_DDPServerFromStr(web_DDPServer value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_DDPServerCopy(web_DDPServer *dst, web_DDPServer src);
#define web_DDPServerCopy(dst, src) _web_DDPServerCopy(dst, web_DDPServer(src))
CORTO_WEB_EXPORT corto_int16 _web_DDPServerCompare(web_DDPServer dst, web_DDPServer src);
#define web_DDPServerCompare(dst, src) _web_DDPServerCompare(web_DDPServer(dst), web_DDPServer(src))

/* /corto/web/DDPServer/Collection */
CORTO_WEB_EXPORT web_DDPServer_Collection _web_DDPServer_CollectionCreate(corto_object obj, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_CollectionCreate(obj, value, meta, scope) _web_DDPServer_CollectionCreate(obj, value, meta, scope)
CORTO_WEB_EXPORT web_DDPServer_Collection _web_DDPServer_CollectionCreateChild(corto_object _parent, corto_string _name, corto_object obj, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_CollectionCreateChild(_parent, _name, obj, value, meta, scope) _web_DDPServer_CollectionCreateChild(_parent, _name, obj, value, meta, scope)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_CollectionUpdate(web_DDPServer_Collection _this, corto_object obj, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_CollectionUpdate(_this, obj, value, meta, scope) _web_DDPServer_CollectionUpdate(web_DDPServer_Collection(_this), obj, value, meta, scope)

CORTO_WEB_EXPORT web_DDPServer_Collection _web_DDPServer_CollectionDeclare(void);
#define web_DDPServer_CollectionDeclare() _web_DDPServer_CollectionDeclare()
CORTO_WEB_EXPORT web_DDPServer_Collection _web_DDPServer_CollectionDeclareChild(corto_object _parent, corto_string _name);
#define web_DDPServer_CollectionDeclareChild(_parent, _name) _web_DDPServer_CollectionDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_CollectionDefine(web_DDPServer_Collection _this, corto_object obj, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_CollectionDefine(_this, obj, value, meta, scope) _web_DDPServer_CollectionDefine(web_DDPServer_Collection(_this), obj, value, meta, scope)
CORTO_WEB_EXPORT void _web_DDPServer_CollectionSet(web_DDPServer_Collection _this, corto_object obj, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_CollectionSet(_this, obj, value, meta, scope) _web_DDPServer_CollectionSet(web_DDPServer_Collection(_this), obj, value, meta, scope)
CORTO_WEB_EXPORT corto_string _web_DDPServer_CollectionStr(web_DDPServer_Collection value);
#define web_DDPServer_CollectionStr(value) _web_DDPServer_CollectionStr(web_DDPServer_Collection(value))
CORTO_WEB_EXPORT web_DDPServer_Collection web_DDPServer_CollectionFromStr(web_DDPServer_Collection value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_CollectionCopy(web_DDPServer_Collection *dst, web_DDPServer_Collection src);
#define web_DDPServer_CollectionCopy(dst, src) _web_DDPServer_CollectionCopy(dst, web_DDPServer_Collection(src))
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_CollectionCompare(web_DDPServer_Collection dst, web_DDPServer_Collection src);
#define web_DDPServer_CollectionCompare(dst, src) _web_DDPServer_CollectionCompare(web_DDPServer_Collection(dst), web_DDPServer_Collection(src))

/* /corto/web/DDPServer/Publication */
CORTO_WEB_EXPORT web_DDPServer_Publication _web_DDPServer_PublicationCreate(corto_object scope);
#define web_DDPServer_PublicationCreate(scope) _web_DDPServer_PublicationCreate(scope)
CORTO_WEB_EXPORT web_DDPServer_Publication _web_DDPServer_PublicationCreateChild(corto_object _parent, corto_string _name, corto_object scope);
#define web_DDPServer_PublicationCreateChild(_parent, _name, scope) _web_DDPServer_PublicationCreateChild(_parent, _name, scope)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_PublicationUpdate(web_DDPServer_Publication _this, corto_object scope);
#define web_DDPServer_PublicationUpdate(_this, scope) _web_DDPServer_PublicationUpdate(web_DDPServer_Publication(_this), scope)

CORTO_WEB_EXPORT web_DDPServer_Publication _web_DDPServer_PublicationDeclare(void);
#define web_DDPServer_PublicationDeclare() _web_DDPServer_PublicationDeclare()
CORTO_WEB_EXPORT web_DDPServer_Publication _web_DDPServer_PublicationDeclareChild(corto_object _parent, corto_string _name);
#define web_DDPServer_PublicationDeclareChild(_parent, _name) _web_DDPServer_PublicationDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_PublicationDefine(web_DDPServer_Publication _this, corto_object scope);
#define web_DDPServer_PublicationDefine(_this, scope) _web_DDPServer_PublicationDefine(web_DDPServer_Publication(_this), scope)
CORTO_WEB_EXPORT void _web_DDPServer_PublicationSet(web_DDPServer_Publication _this, corto_object scope);
#define web_DDPServer_PublicationSet(_this, scope) _web_DDPServer_PublicationSet(web_DDPServer_Publication(_this), scope)
CORTO_WEB_EXPORT corto_string _web_DDPServer_PublicationStr(web_DDPServer_Publication value);
#define web_DDPServer_PublicationStr(value) _web_DDPServer_PublicationStr(web_DDPServer_Publication(value))
CORTO_WEB_EXPORT web_DDPServer_Publication web_DDPServer_PublicationFromStr(web_DDPServer_Publication value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_PublicationCopy(web_DDPServer_Publication *dst, web_DDPServer_Publication src);
#define web_DDPServer_PublicationCopy(dst, src) _web_DDPServer_PublicationCopy(dst, web_DDPServer_Publication(src))
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_PublicationCompare(web_DDPServer_Publication dst, web_DDPServer_Publication src);
#define web_DDPServer_PublicationCompare(dst, src) _web_DDPServer_PublicationCompare(web_DDPServer_Publication(dst), web_DDPServer_Publication(src))

/* /corto/web/DDPServer/readyEvent */
CORTO_WEB_EXPORT web_DDPServer_readyEvent _web_DDPServer_readyEventCreate(corto_uint16 kind);
#define web_DDPServer_readyEventCreate(kind) _web_DDPServer_readyEventCreate(kind)
CORTO_WEB_EXPORT web_DDPServer_readyEvent _web_DDPServer_readyEventCreateChild(corto_object _parent, corto_string _name, corto_uint16 kind);
#define web_DDPServer_readyEventCreateChild(_parent, _name, kind) _web_DDPServer_readyEventCreateChild(_parent, _name, kind)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_readyEventUpdate(web_DDPServer_readyEvent _this, corto_uint16 kind);
#define web_DDPServer_readyEventUpdate(_this, kind) _web_DDPServer_readyEventUpdate(web_DDPServer_readyEvent(_this), kind)

CORTO_WEB_EXPORT web_DDPServer_readyEvent _web_DDPServer_readyEventDeclare(void);
#define web_DDPServer_readyEventDeclare() _web_DDPServer_readyEventDeclare()
CORTO_WEB_EXPORT web_DDPServer_readyEvent _web_DDPServer_readyEventDeclareChild(corto_object _parent, corto_string _name);
#define web_DDPServer_readyEventDeclareChild(_parent, _name) _web_DDPServer_readyEventDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_readyEventDefine(web_DDPServer_readyEvent _this, corto_uint16 kind);
#define web_DDPServer_readyEventDefine(_this, kind) _web_DDPServer_readyEventDefine(web_DDPServer_readyEvent(_this), kind)
CORTO_WEB_EXPORT void _web_DDPServer_readyEventSet(web_DDPServer_readyEvent _this, corto_uint16 kind);
#define web_DDPServer_readyEventSet(_this, kind) _web_DDPServer_readyEventSet(web_DDPServer_readyEvent(_this), kind)
CORTO_WEB_EXPORT corto_string _web_DDPServer_readyEventStr(web_DDPServer_readyEvent value);
#define web_DDPServer_readyEventStr(value) _web_DDPServer_readyEventStr(web_DDPServer_readyEvent(value))
CORTO_WEB_EXPORT web_DDPServer_readyEvent web_DDPServer_readyEventFromStr(web_DDPServer_readyEvent value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_readyEventCopy(web_DDPServer_readyEvent *dst, web_DDPServer_readyEvent src);
#define web_DDPServer_readyEventCopy(dst, src) _web_DDPServer_readyEventCopy(dst, web_DDPServer_readyEvent(src))
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_readyEventCompare(web_DDPServer_readyEvent dst, web_DDPServer_readyEvent src);
#define web_DDPServer_readyEventCompare(dst, src) _web_DDPServer_readyEventCompare(web_DDPServer_readyEvent(dst), web_DDPServer_readyEvent(src))

/* /corto/web/DDPServer/Session */
CORTO_WEB_EXPORT web_DDPServer_Session _web_DDPServer_SessionCreate(web_SockJsServer_Connection conn);
#define web_DDPServer_SessionCreate(conn) _web_DDPServer_SessionCreate(web_SockJsServer_Connection(conn))
CORTO_WEB_EXPORT web_DDPServer_Session _web_DDPServer_SessionCreateChild(corto_object _parent, corto_string _name, web_SockJsServer_Connection conn);
#define web_DDPServer_SessionCreateChild(_parent, _name, conn) _web_DDPServer_SessionCreateChild(_parent, _name, web_SockJsServer_Connection(conn))
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SessionUpdate(web_DDPServer_Session _this, web_SockJsServer_Connection conn);
#define web_DDPServer_SessionUpdate(_this, conn) _web_DDPServer_SessionUpdate(web_DDPServer_Session(_this), web_SockJsServer_Connection(conn))

CORTO_WEB_EXPORT web_DDPServer_Session _web_DDPServer_SessionDeclare(void);
#define web_DDPServer_SessionDeclare() _web_DDPServer_SessionDeclare()
CORTO_WEB_EXPORT web_DDPServer_Session _web_DDPServer_SessionDeclareChild(corto_object _parent, corto_string _name);
#define web_DDPServer_SessionDeclareChild(_parent, _name) _web_DDPServer_SessionDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SessionDefine(web_DDPServer_Session _this, web_SockJsServer_Connection conn);
#define web_DDPServer_SessionDefine(_this, conn) _web_DDPServer_SessionDefine(web_DDPServer_Session(_this), web_SockJsServer_Connection(conn))
CORTO_WEB_EXPORT void _web_DDPServer_SessionSet(web_DDPServer_Session _this, web_SockJsServer_Connection conn);
#define web_DDPServer_SessionSet(_this, conn) _web_DDPServer_SessionSet(web_DDPServer_Session(_this), web_SockJsServer_Connection(conn))
CORTO_WEB_EXPORT corto_string _web_DDPServer_SessionStr(web_DDPServer_Session value);
#define web_DDPServer_SessionStr(value) _web_DDPServer_SessionStr(web_DDPServer_Session(value))
CORTO_WEB_EXPORT web_DDPServer_Session web_DDPServer_SessionFromStr(web_DDPServer_Session value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SessionCopy(web_DDPServer_Session *dst, web_DDPServer_Session src);
#define web_DDPServer_SessionCopy(dst, src) _web_DDPServer_SessionCopy(dst, web_DDPServer_Session(src))
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SessionCompare(web_DDPServer_Session dst, web_DDPServer_Session src);
#define web_DDPServer_SessionCompare(dst, src) _web_DDPServer_SessionCompare(web_DDPServer_Session(dst), web_DDPServer_Session(src))

/* /corto/web/DDPServer/Subscription */
CORTO_WEB_EXPORT web_DDPServer_Subscription _web_DDPServer_SubscriptionCreate(web_DDPServer_Publication pub, corto_string id, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_SubscriptionCreate(pub, id, value, meta, scope) _web_DDPServer_SubscriptionCreate(web_DDPServer_Publication(pub), id, value, meta, scope)
CORTO_WEB_EXPORT web_DDPServer_Subscription _web_DDPServer_SubscriptionCreateChild(corto_object _parent, corto_string _name, web_DDPServer_Publication pub, corto_string id, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_SubscriptionCreateChild(_parent, _name, pub, id, value, meta, scope) _web_DDPServer_SubscriptionCreateChild(_parent, _name, web_DDPServer_Publication(pub), id, value, meta, scope)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SubscriptionUpdate(web_DDPServer_Subscription _this, web_DDPServer_Publication pub, corto_string id, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_SubscriptionUpdate(_this, pub, id, value, meta, scope) _web_DDPServer_SubscriptionUpdate(web_DDPServer_Subscription(_this), web_DDPServer_Publication(pub), id, value, meta, scope)

CORTO_WEB_EXPORT web_DDPServer_Subscription _web_DDPServer_SubscriptionDeclare(void);
#define web_DDPServer_SubscriptionDeclare() _web_DDPServer_SubscriptionDeclare()
CORTO_WEB_EXPORT web_DDPServer_Subscription _web_DDPServer_SubscriptionDeclareChild(corto_object _parent, corto_string _name);
#define web_DDPServer_SubscriptionDeclareChild(_parent, _name) _web_DDPServer_SubscriptionDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SubscriptionDefine(web_DDPServer_Subscription _this, web_DDPServer_Publication pub, corto_string id, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_SubscriptionDefine(_this, pub, id, value, meta, scope) _web_DDPServer_SubscriptionDefine(web_DDPServer_Subscription(_this), web_DDPServer_Publication(pub), id, value, meta, scope)
CORTO_WEB_EXPORT void _web_DDPServer_SubscriptionSet(web_DDPServer_Subscription _this, web_DDPServer_Publication pub, corto_string id, corto_bool value, corto_bool meta, corto_bool scope);
#define web_DDPServer_SubscriptionSet(_this, pub, id, value, meta, scope) _web_DDPServer_SubscriptionSet(web_DDPServer_Subscription(_this), web_DDPServer_Publication(pub), id, value, meta, scope)
CORTO_WEB_EXPORT corto_string _web_DDPServer_SubscriptionStr(web_DDPServer_Subscription value);
#define web_DDPServer_SubscriptionStr(value) _web_DDPServer_SubscriptionStr(web_DDPServer_Subscription(value))
CORTO_WEB_EXPORT web_DDPServer_Subscription web_DDPServer_SubscriptionFromStr(web_DDPServer_Subscription value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SubscriptionCopy(web_DDPServer_Subscription *dst, web_DDPServer_Subscription src);
#define web_DDPServer_SubscriptionCopy(dst, src) _web_DDPServer_SubscriptionCopy(dst, web_DDPServer_Subscription(src))
CORTO_WEB_EXPORT corto_int16 _web_DDPServer_SubscriptionCompare(web_DDPServer_Subscription dst, web_DDPServer_Subscription src);
#define web_DDPServer_SubscriptionCompare(dst, src) _web_DDPServer_SubscriptionCompare(web_DDPServer_Subscription(dst), web_DDPServer_Subscription(src))

/* /corto/web/handler */
CORTO_WEB_EXPORT web_handler* _web_handlerCreate(corto_object instance, corto_function procedure);
#define web_handlerCreate(instance, procedure) _web_handlerCreate(instance, corto_function(procedure))
CORTO_WEB_EXPORT web_handler* _web_handlerCreateChild(corto_object _parent, corto_string _name, corto_object instance, corto_function procedure);
#define web_handlerCreateChild(_parent, _name, instance, procedure) _web_handlerCreateChild(_parent, _name, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_int16 _web_handlerUpdate(web_handler* _this, corto_object instance, corto_function procedure);
#define web_handlerUpdate(_this, instance, procedure) _web_handlerUpdate(_this, instance, corto_function(procedure))

CORTO_WEB_EXPORT web_handler* _web_handlerDeclare(void);
#define web_handlerDeclare() _web_handlerDeclare()
CORTO_WEB_EXPORT web_handler* _web_handlerDeclareChild(corto_object _parent, corto_string _name);
#define web_handlerDeclareChild(_parent, _name) _web_handlerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_handlerDefine(web_handler* _this, corto_object instance, corto_function procedure);
#define web_handlerDefine(_this, instance, procedure) _web_handlerDefine(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT void _web_handlerSet(web_handler* _this, corto_object instance, corto_function procedure);
#define web_handlerSet(_this, instance, procedure) _web_handlerSet(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_string _web_handlerStr(web_handler* value);
#define web_handlerStr(value) _web_handlerStr(value)
CORTO_WEB_EXPORT web_handler* web_handlerFromStr(web_handler* value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_handlerCopy(web_handler* *dst, web_handler* src);
#define web_handlerCopy(dst, src) _web_handlerCopy(dst, src)
CORTO_WEB_EXPORT corto_int16 _web_handlerCompare(web_handler* dst, web_handler* src);
#define web_handlerCompare(dst, src) _web_handlerCompare(dst, src)

CORTO_WEB_EXPORT corto_int16 _web_handlerInit(web_handler* value);
#define web_handlerInit(value) _web_handlerInit(value)
CORTO_WEB_EXPORT corto_int16 _web_handlerDeinit(web_handler* value);
#define web_handlerDeinit(value) _web_handlerDeinit(value)

corto_int16 web_handlerCall(web_handler *_delegate, web_WebSocketConnection conn);
/* /corto/web/messageHandler */
CORTO_WEB_EXPORT web_messageHandler* _web_messageHandlerCreate(corto_object instance, corto_function procedure);
#define web_messageHandlerCreate(instance, procedure) _web_messageHandlerCreate(instance, corto_function(procedure))
CORTO_WEB_EXPORT web_messageHandler* _web_messageHandlerCreateChild(corto_object _parent, corto_string _name, corto_object instance, corto_function procedure);
#define web_messageHandlerCreateChild(_parent, _name, instance, procedure) _web_messageHandlerCreateChild(_parent, _name, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_int16 _web_messageHandlerUpdate(web_messageHandler* _this, corto_object instance, corto_function procedure);
#define web_messageHandlerUpdate(_this, instance, procedure) _web_messageHandlerUpdate(_this, instance, corto_function(procedure))

CORTO_WEB_EXPORT web_messageHandler* _web_messageHandlerDeclare(void);
#define web_messageHandlerDeclare() _web_messageHandlerDeclare()
CORTO_WEB_EXPORT web_messageHandler* _web_messageHandlerDeclareChild(corto_object _parent, corto_string _name);
#define web_messageHandlerDeclareChild(_parent, _name) _web_messageHandlerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_messageHandlerDefine(web_messageHandler* _this, corto_object instance, corto_function procedure);
#define web_messageHandlerDefine(_this, instance, procedure) _web_messageHandlerDefine(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT void _web_messageHandlerSet(web_messageHandler* _this, corto_object instance, corto_function procedure);
#define web_messageHandlerSet(_this, instance, procedure) _web_messageHandlerSet(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_string _web_messageHandlerStr(web_messageHandler* value);
#define web_messageHandlerStr(value) _web_messageHandlerStr(value)
CORTO_WEB_EXPORT web_messageHandler* web_messageHandlerFromStr(web_messageHandler* value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_messageHandlerCopy(web_messageHandler* *dst, web_messageHandler* src);
#define web_messageHandlerCopy(dst, src) _web_messageHandlerCopy(dst, src)
CORTO_WEB_EXPORT corto_int16 _web_messageHandlerCompare(web_messageHandler* dst, web_messageHandler* src);
#define web_messageHandlerCompare(dst, src) _web_messageHandlerCompare(dst, src)

CORTO_WEB_EXPORT corto_int16 _web_messageHandlerInit(web_messageHandler* value);
#define web_messageHandlerInit(value) _web_messageHandlerInit(value)
CORTO_WEB_EXPORT corto_int16 _web_messageHandlerDeinit(web_messageHandler* value);
#define web_messageHandlerDeinit(value) _web_messageHandlerDeinit(value)

corto_int16 web_messageHandlerCall(web_messageHandler *_delegate, web_WebSocketConnection conn, corto_string message);
/* /corto/web/SockJsServer */
CORTO_WEB_EXPORT web_SockJsServer _web_SockJsServerCreate(corto_uint16 port);
#define web_SockJsServerCreate(port) _web_SockJsServerCreate(port)
CORTO_WEB_EXPORT web_SockJsServer _web_SockJsServerCreateChild(corto_object _parent, corto_string _name, corto_uint16 port);
#define web_SockJsServerCreateChild(_parent, _name, port) _web_SockJsServerCreateChild(_parent, _name, port)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServerUpdate(web_SockJsServer _this, corto_uint16 port);
#define web_SockJsServerUpdate(_this, port) _web_SockJsServerUpdate(web_SockJsServer(_this), port)

CORTO_WEB_EXPORT web_SockJsServer _web_SockJsServerDeclare(void);
#define web_SockJsServerDeclare() _web_SockJsServerDeclare()
CORTO_WEB_EXPORT web_SockJsServer _web_SockJsServerDeclareChild(corto_object _parent, corto_string _name);
#define web_SockJsServerDeclareChild(_parent, _name) _web_SockJsServerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServerDefine(web_SockJsServer _this, corto_uint16 port);
#define web_SockJsServerDefine(_this, port) _web_SockJsServerDefine(web_SockJsServer(_this), port)
CORTO_WEB_EXPORT void _web_SockJsServerSet(web_SockJsServer _this, corto_uint16 port);
#define web_SockJsServerSet(_this, port) _web_SockJsServerSet(web_SockJsServer(_this), port)
CORTO_WEB_EXPORT corto_string _web_SockJsServerStr(web_SockJsServer value);
#define web_SockJsServerStr(value) _web_SockJsServerStr(web_SockJsServer(value))
CORTO_WEB_EXPORT web_SockJsServer web_SockJsServerFromStr(web_SockJsServer value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_SockJsServerCopy(web_SockJsServer *dst, web_SockJsServer src);
#define web_SockJsServerCopy(dst, src) _web_SockJsServerCopy(dst, web_SockJsServer(src))
CORTO_WEB_EXPORT corto_int16 _web_SockJsServerCompare(web_SockJsServer dst, web_SockJsServer src);
#define web_SockJsServerCompare(dst, src) _web_SockJsServerCompare(web_SockJsServer(dst), web_SockJsServer(src))

/* /corto/web/SockJsServer/Connection */
CORTO_WEB_EXPORT web_SockJsServer_Connection _web_SockJsServer_ConnectionCreate(corto_object data);
#define web_SockJsServer_ConnectionCreate(data) _web_SockJsServer_ConnectionCreate(data)
CORTO_WEB_EXPORT web_SockJsServer_Connection _web_SockJsServer_ConnectionCreateChild(corto_object _parent, corto_string _name, corto_object data);
#define web_SockJsServer_ConnectionCreateChild(_parent, _name, data) _web_SockJsServer_ConnectionCreateChild(_parent, _name, data)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_ConnectionUpdate(web_SockJsServer_Connection _this, corto_object data);
#define web_SockJsServer_ConnectionUpdate(_this, data) _web_SockJsServer_ConnectionUpdate(web_SockJsServer_Connection(_this), data)

CORTO_WEB_EXPORT web_SockJsServer_Connection _web_SockJsServer_ConnectionDeclare(void);
#define web_SockJsServer_ConnectionDeclare() _web_SockJsServer_ConnectionDeclare()
CORTO_WEB_EXPORT web_SockJsServer_Connection _web_SockJsServer_ConnectionDeclareChild(corto_object _parent, corto_string _name);
#define web_SockJsServer_ConnectionDeclareChild(_parent, _name) _web_SockJsServer_ConnectionDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_ConnectionDefine(web_SockJsServer_Connection _this, corto_object data);
#define web_SockJsServer_ConnectionDefine(_this, data) _web_SockJsServer_ConnectionDefine(web_SockJsServer_Connection(_this), data)
CORTO_WEB_EXPORT void _web_SockJsServer_ConnectionSet(web_SockJsServer_Connection _this, corto_object data);
#define web_SockJsServer_ConnectionSet(_this, data) _web_SockJsServer_ConnectionSet(web_SockJsServer_Connection(_this), data)
CORTO_WEB_EXPORT corto_string _web_SockJsServer_ConnectionStr(web_SockJsServer_Connection value);
#define web_SockJsServer_ConnectionStr(value) _web_SockJsServer_ConnectionStr(web_SockJsServer_Connection(value))
CORTO_WEB_EXPORT web_SockJsServer_Connection web_SockJsServer_ConnectionFromStr(web_SockJsServer_Connection value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_ConnectionCopy(web_SockJsServer_Connection *dst, web_SockJsServer_Connection src);
#define web_SockJsServer_ConnectionCopy(dst, src) _web_SockJsServer_ConnectionCopy(dst, web_SockJsServer_Connection(src))
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_ConnectionCompare(web_SockJsServer_Connection dst, web_SockJsServer_Connection src);
#define web_SockJsServer_ConnectionCompare(dst, src) _web_SockJsServer_ConnectionCompare(web_SockJsServer_Connection(dst), web_SockJsServer_Connection(src))

/* /corto/web/SockJsServer/handler */
CORTO_WEB_EXPORT web_SockJsServer_handler* _web_SockJsServer_handlerCreate(corto_object instance, corto_function procedure);
#define web_SockJsServer_handlerCreate(instance, procedure) _web_SockJsServer_handlerCreate(instance, corto_function(procedure))
CORTO_WEB_EXPORT web_SockJsServer_handler* _web_SockJsServer_handlerCreateChild(corto_object _parent, corto_string _name, corto_object instance, corto_function procedure);
#define web_SockJsServer_handlerCreateChild(_parent, _name, instance, procedure) _web_SockJsServer_handlerCreateChild(_parent, _name, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_handlerUpdate(web_SockJsServer_handler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_handlerUpdate(_this, instance, procedure) _web_SockJsServer_handlerUpdate(_this, instance, corto_function(procedure))

CORTO_WEB_EXPORT web_SockJsServer_handler* _web_SockJsServer_handlerDeclare(void);
#define web_SockJsServer_handlerDeclare() _web_SockJsServer_handlerDeclare()
CORTO_WEB_EXPORT web_SockJsServer_handler* _web_SockJsServer_handlerDeclareChild(corto_object _parent, corto_string _name);
#define web_SockJsServer_handlerDeclareChild(_parent, _name) _web_SockJsServer_handlerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_handlerDefine(web_SockJsServer_handler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_handlerDefine(_this, instance, procedure) _web_SockJsServer_handlerDefine(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT void _web_SockJsServer_handlerSet(web_SockJsServer_handler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_handlerSet(_this, instance, procedure) _web_SockJsServer_handlerSet(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_string _web_SockJsServer_handlerStr(web_SockJsServer_handler* value);
#define web_SockJsServer_handlerStr(value) _web_SockJsServer_handlerStr(value)
CORTO_WEB_EXPORT web_SockJsServer_handler* web_SockJsServer_handlerFromStr(web_SockJsServer_handler* value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_handlerCopy(web_SockJsServer_handler* *dst, web_SockJsServer_handler* src);
#define web_SockJsServer_handlerCopy(dst, src) _web_SockJsServer_handlerCopy(dst, src)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_handlerCompare(web_SockJsServer_handler* dst, web_SockJsServer_handler* src);
#define web_SockJsServer_handlerCompare(dst, src) _web_SockJsServer_handlerCompare(dst, src)

CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_handlerInit(web_SockJsServer_handler* value);
#define web_SockJsServer_handlerInit(value) _web_SockJsServer_handlerInit(value)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_handlerDeinit(web_SockJsServer_handler* value);
#define web_SockJsServer_handlerDeinit(value) _web_SockJsServer_handlerDeinit(value)

corto_int16 web_SockJsServer_handlerCall(web_SockJsServer_handler *_delegate, web_SockJsServer_Connection conn);
/* /corto/web/SockJsServer/messageHandler */
CORTO_WEB_EXPORT web_SockJsServer_messageHandler* _web_SockJsServer_messageHandlerCreate(corto_object instance, corto_function procedure);
#define web_SockJsServer_messageHandlerCreate(instance, procedure) _web_SockJsServer_messageHandlerCreate(instance, corto_function(procedure))
CORTO_WEB_EXPORT web_SockJsServer_messageHandler* _web_SockJsServer_messageHandlerCreateChild(corto_object _parent, corto_string _name, corto_object instance, corto_function procedure);
#define web_SockJsServer_messageHandlerCreateChild(_parent, _name, instance, procedure) _web_SockJsServer_messageHandlerCreateChild(_parent, _name, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_messageHandlerUpdate(web_SockJsServer_messageHandler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_messageHandlerUpdate(_this, instance, procedure) _web_SockJsServer_messageHandlerUpdate(_this, instance, corto_function(procedure))

CORTO_WEB_EXPORT web_SockJsServer_messageHandler* _web_SockJsServer_messageHandlerDeclare(void);
#define web_SockJsServer_messageHandlerDeclare() _web_SockJsServer_messageHandlerDeclare()
CORTO_WEB_EXPORT web_SockJsServer_messageHandler* _web_SockJsServer_messageHandlerDeclareChild(corto_object _parent, corto_string _name);
#define web_SockJsServer_messageHandlerDeclareChild(_parent, _name) _web_SockJsServer_messageHandlerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_messageHandlerDefine(web_SockJsServer_messageHandler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_messageHandlerDefine(_this, instance, procedure) _web_SockJsServer_messageHandlerDefine(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT void _web_SockJsServer_messageHandlerSet(web_SockJsServer_messageHandler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_messageHandlerSet(_this, instance, procedure) _web_SockJsServer_messageHandlerSet(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_string _web_SockJsServer_messageHandlerStr(web_SockJsServer_messageHandler* value);
#define web_SockJsServer_messageHandlerStr(value) _web_SockJsServer_messageHandlerStr(value)
CORTO_WEB_EXPORT web_SockJsServer_messageHandler* web_SockJsServer_messageHandlerFromStr(web_SockJsServer_messageHandler* value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_messageHandlerCopy(web_SockJsServer_messageHandler* *dst, web_SockJsServer_messageHandler* src);
#define web_SockJsServer_messageHandlerCopy(dst, src) _web_SockJsServer_messageHandlerCopy(dst, src)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_messageHandlerCompare(web_SockJsServer_messageHandler* dst, web_SockJsServer_messageHandler* src);
#define web_SockJsServer_messageHandlerCompare(dst, src) _web_SockJsServer_messageHandlerCompare(dst, src)

CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_messageHandlerInit(web_SockJsServer_messageHandler* value);
#define web_SockJsServer_messageHandlerInit(value) _web_SockJsServer_messageHandlerInit(value)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_messageHandlerDeinit(web_SockJsServer_messageHandler* value);
#define web_SockJsServer_messageHandlerDeinit(value) _web_SockJsServer_messageHandlerDeinit(value)

corto_int16 web_SockJsServer_messageHandlerCall(web_SockJsServer_messageHandler *_delegate, web_SockJsServer_Connection conn, corto_string msg);
/* /corto/web/SockJsServer/uriHandler */
CORTO_WEB_EXPORT web_SockJsServer_uriHandler* _web_SockJsServer_uriHandlerCreate(corto_object instance, corto_function procedure);
#define web_SockJsServer_uriHandlerCreate(instance, procedure) _web_SockJsServer_uriHandlerCreate(instance, corto_function(procedure))
CORTO_WEB_EXPORT web_SockJsServer_uriHandler* _web_SockJsServer_uriHandlerCreateChild(corto_object _parent, corto_string _name, corto_object instance, corto_function procedure);
#define web_SockJsServer_uriHandlerCreateChild(_parent, _name, instance, procedure) _web_SockJsServer_uriHandlerCreateChild(_parent, _name, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_uriHandlerUpdate(web_SockJsServer_uriHandler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_uriHandlerUpdate(_this, instance, procedure) _web_SockJsServer_uriHandlerUpdate(_this, instance, corto_function(procedure))

CORTO_WEB_EXPORT web_SockJsServer_uriHandler* _web_SockJsServer_uriHandlerDeclare(void);
#define web_SockJsServer_uriHandlerDeclare() _web_SockJsServer_uriHandlerDeclare()
CORTO_WEB_EXPORT web_SockJsServer_uriHandler* _web_SockJsServer_uriHandlerDeclareChild(corto_object _parent, corto_string _name);
#define web_SockJsServer_uriHandlerDeclareChild(_parent, _name) _web_SockJsServer_uriHandlerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_uriHandlerDefine(web_SockJsServer_uriHandler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_uriHandlerDefine(_this, instance, procedure) _web_SockJsServer_uriHandlerDefine(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT void _web_SockJsServer_uriHandlerSet(web_SockJsServer_uriHandler* _this, corto_object instance, corto_function procedure);
#define web_SockJsServer_uriHandlerSet(_this, instance, procedure) _web_SockJsServer_uriHandlerSet(_this, instance, corto_function(procedure))
CORTO_WEB_EXPORT corto_string _web_SockJsServer_uriHandlerStr(web_SockJsServer_uriHandler* value);
#define web_SockJsServer_uriHandlerStr(value) _web_SockJsServer_uriHandlerStr(value)
CORTO_WEB_EXPORT web_SockJsServer_uriHandler* web_SockJsServer_uriHandlerFromStr(web_SockJsServer_uriHandler* value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_uriHandlerCopy(web_SockJsServer_uriHandler* *dst, web_SockJsServer_uriHandler* src);
#define web_SockJsServer_uriHandlerCopy(dst, src) _web_SockJsServer_uriHandlerCopy(dst, src)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_uriHandlerCompare(web_SockJsServer_uriHandler* dst, web_SockJsServer_uriHandler* src);
#define web_SockJsServer_uriHandlerCompare(dst, src) _web_SockJsServer_uriHandlerCompare(dst, src)

CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_uriHandlerInit(web_SockJsServer_uriHandler* value);
#define web_SockJsServer_uriHandlerInit(value) _web_SockJsServer_uriHandlerInit(value)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_uriHandlerDeinit(web_SockJsServer_uriHandler* value);
#define web_SockJsServer_uriHandlerDeinit(value) _web_SockJsServer_uriHandlerDeinit(value)

corto_int16 web_SockJsServer_uriHandlerCall(web_SockJsServer_uriHandler *_delegate, web_SockJsServer_UriRequest conn, corto_string uri);
/* /corto/web/SockJsServer/UriRequest */
CORTO_WEB_EXPORT web_SockJsServer_UriRequest* _web_SockJsServer_UriRequestCreate(void);
#define web_SockJsServer_UriRequestCreate() _web_SockJsServer_UriRequestCreate()
CORTO_WEB_EXPORT web_SockJsServer_UriRequest* _web_SockJsServer_UriRequestCreateChild(corto_object _parent, corto_string _name);
#define web_SockJsServer_UriRequestCreateChild(_parent, _name) _web_SockJsServer_UriRequestCreateChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_UriRequestUpdate(web_SockJsServer_UriRequest* _this);
#define web_SockJsServer_UriRequestUpdate(_this) _web_SockJsServer_UriRequestUpdate(_this)

CORTO_WEB_EXPORT web_SockJsServer_UriRequest* _web_SockJsServer_UriRequestDeclare(void);
#define web_SockJsServer_UriRequestDeclare() _web_SockJsServer_UriRequestDeclare()
CORTO_WEB_EXPORT web_SockJsServer_UriRequest* _web_SockJsServer_UriRequestDeclareChild(corto_object _parent, corto_string _name);
#define web_SockJsServer_UriRequestDeclareChild(_parent, _name) _web_SockJsServer_UriRequestDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_UriRequestDefine(web_SockJsServer_UriRequest* _this);
#define web_SockJsServer_UriRequestDefine(_this) _web_SockJsServer_UriRequestDefine(_this)
CORTO_WEB_EXPORT void _web_SockJsServer_UriRequestSet(web_SockJsServer_UriRequest* _this);
#define web_SockJsServer_UriRequestSet(_this) _web_SockJsServer_UriRequestSet(_this)
CORTO_WEB_EXPORT corto_string _web_SockJsServer_UriRequestStr(web_SockJsServer_UriRequest* value);
#define web_SockJsServer_UriRequestStr(value) _web_SockJsServer_UriRequestStr(value)
CORTO_WEB_EXPORT web_SockJsServer_UriRequest* web_SockJsServer_UriRequestFromStr(web_SockJsServer_UriRequest* value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_UriRequestCopy(web_SockJsServer_UriRequest* *dst, web_SockJsServer_UriRequest* src);
#define web_SockJsServer_UriRequestCopy(dst, src) _web_SockJsServer_UriRequestCopy(dst, src)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_UriRequestCompare(web_SockJsServer_UriRequest* dst, web_SockJsServer_UriRequest* src);
#define web_SockJsServer_UriRequestCompare(dst, src) _web_SockJsServer_UriRequestCompare(dst, src)

CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_UriRequestInit(web_SockJsServer_UriRequest* value);
#define web_SockJsServer_UriRequestInit(value) _web_SockJsServer_UriRequestInit(value)
CORTO_WEB_EXPORT corto_int16 _web_SockJsServer_UriRequestDeinit(web_SockJsServer_UriRequest* value);
#define web_SockJsServer_UriRequestDeinit(value) _web_SockJsServer_UriRequestDeinit(value)

/* /corto/web/WebSocketConnection */
CORTO_WEB_EXPORT web_WebSocketConnection _web_WebSocketConnectionCreate(corto_object data);
#define web_WebSocketConnectionCreate(data) _web_WebSocketConnectionCreate(data)
CORTO_WEB_EXPORT web_WebSocketConnection _web_WebSocketConnectionCreateChild(corto_object _parent, corto_string _name, corto_object data);
#define web_WebSocketConnectionCreateChild(_parent, _name, data) _web_WebSocketConnectionCreateChild(_parent, _name, data)
CORTO_WEB_EXPORT corto_int16 _web_WebSocketConnectionUpdate(web_WebSocketConnection _this, corto_object data);
#define web_WebSocketConnectionUpdate(_this, data) _web_WebSocketConnectionUpdate(web_WebSocketConnection(_this), data)

CORTO_WEB_EXPORT web_WebSocketConnection _web_WebSocketConnectionDeclare(void);
#define web_WebSocketConnectionDeclare() _web_WebSocketConnectionDeclare()
CORTO_WEB_EXPORT web_WebSocketConnection _web_WebSocketConnectionDeclareChild(corto_object _parent, corto_string _name);
#define web_WebSocketConnectionDeclareChild(_parent, _name) _web_WebSocketConnectionDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_WebSocketConnectionDefine(web_WebSocketConnection _this, corto_object data);
#define web_WebSocketConnectionDefine(_this, data) _web_WebSocketConnectionDefine(web_WebSocketConnection(_this), data)
CORTO_WEB_EXPORT void _web_WebSocketConnectionSet(web_WebSocketConnection _this, corto_object data);
#define web_WebSocketConnectionSet(_this, data) _web_WebSocketConnectionSet(web_WebSocketConnection(_this), data)
CORTO_WEB_EXPORT corto_string _web_WebSocketConnectionStr(web_WebSocketConnection value);
#define web_WebSocketConnectionStr(value) _web_WebSocketConnectionStr(web_WebSocketConnection(value))
CORTO_WEB_EXPORT web_WebSocketConnection web_WebSocketConnectionFromStr(web_WebSocketConnection value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_WebSocketConnectionCopy(web_WebSocketConnection *dst, web_WebSocketConnection src);
#define web_WebSocketConnectionCopy(dst, src) _web_WebSocketConnectionCopy(dst, web_WebSocketConnection(src))
CORTO_WEB_EXPORT corto_int16 _web_WebSocketConnectionCompare(web_WebSocketConnection dst, web_WebSocketConnection src);
#define web_WebSocketConnectionCompare(dst, src) _web_WebSocketConnectionCompare(web_WebSocketConnection(dst), web_WebSocketConnection(src))

/* /corto/web/WebSocketServer */
CORTO_WEB_EXPORT web_WebSocketServer _web_WebSocketServerCreate(corto_uint16 port, web_handler onOpen, web_handler onClose, web_messageHandler onMessage, corto_uint16 pollTimemoutMillis);
#define web_WebSocketServerCreate(port, onOpen, onClose, onMessage, pollTimemoutMillis) _web_WebSocketServerCreate(port, onOpen, onClose, onMessage, pollTimemoutMillis)
CORTO_WEB_EXPORT web_WebSocketServer _web_WebSocketServerCreateChild(corto_object _parent, corto_string _name, corto_uint16 port, web_handler onOpen, web_handler onClose, web_messageHandler onMessage, corto_uint16 pollTimemoutMillis);
#define web_WebSocketServerCreateChild(_parent, _name, port, onOpen, onClose, onMessage, pollTimemoutMillis) _web_WebSocketServerCreateChild(_parent, _name, port, onOpen, onClose, onMessage, pollTimemoutMillis)
CORTO_WEB_EXPORT corto_int16 _web_WebSocketServerUpdate(web_WebSocketServer _this, corto_uint16 port, web_handler onOpen, web_handler onClose, web_messageHandler onMessage, corto_uint16 pollTimemoutMillis);
#define web_WebSocketServerUpdate(_this, port, onOpen, onClose, onMessage, pollTimemoutMillis) _web_WebSocketServerUpdate(web_WebSocketServer(_this), port, onOpen, onClose, onMessage, pollTimemoutMillis)

CORTO_WEB_EXPORT web_WebSocketServer _web_WebSocketServerDeclare(void);
#define web_WebSocketServerDeclare() _web_WebSocketServerDeclare()
CORTO_WEB_EXPORT web_WebSocketServer _web_WebSocketServerDeclareChild(corto_object _parent, corto_string _name);
#define web_WebSocketServerDeclareChild(_parent, _name) _web_WebSocketServerDeclareChild(_parent, _name)
CORTO_WEB_EXPORT corto_int16 _web_WebSocketServerDefine(web_WebSocketServer _this, corto_uint16 port, web_handler onOpen, web_handler onClose, web_messageHandler onMessage, corto_uint16 pollTimemoutMillis);
#define web_WebSocketServerDefine(_this, port, onOpen, onClose, onMessage, pollTimemoutMillis) _web_WebSocketServerDefine(web_WebSocketServer(_this), port, onOpen, onClose, onMessage, pollTimemoutMillis)
CORTO_WEB_EXPORT void _web_WebSocketServerSet(web_WebSocketServer _this, corto_uint16 port, web_handler onOpen, web_handler onClose, web_messageHandler onMessage, corto_uint16 pollTimemoutMillis);
#define web_WebSocketServerSet(_this, port, onOpen, onClose, onMessage, pollTimemoutMillis) _web_WebSocketServerSet(web_WebSocketServer(_this), port, onOpen, onClose, onMessage, pollTimemoutMillis)
CORTO_WEB_EXPORT corto_string _web_WebSocketServerStr(web_WebSocketServer value);
#define web_WebSocketServerStr(value) _web_WebSocketServerStr(web_WebSocketServer(value))
CORTO_WEB_EXPORT web_WebSocketServer web_WebSocketServerFromStr(web_WebSocketServer value, corto_string str);
CORTO_WEB_EXPORT corto_int16 _web_WebSocketServerCopy(web_WebSocketServer *dst, web_WebSocketServer src);
#define web_WebSocketServerCopy(dst, src) _web_WebSocketServerCopy(dst, web_WebSocketServer(src))
CORTO_WEB_EXPORT corto_int16 _web_WebSocketServerCompare(web_WebSocketServer dst, web_WebSocketServer src);
#define web_WebSocketServerCompare(dst, src) _web_WebSocketServerCompare(web_WebSocketServer(dst), web_WebSocketServer(src))


/* <0x7fb440c5c1b8> */
#define corto_eventListForeach(list, elem) \
    corto_iter elem##_iter = corto_llIter(list);\
    corto_event elem;\
    while(corto_iterHasNext(&elem##_iter) ? elem = corto_iterNext(&elem##_iter), TRUE : FALSE)

CORTO_WEB_EXPORT void corto_eventListInsert(corto_eventList list, corto_event element);
CORTO_WEB_EXPORT void corto_eventListAppend(corto_eventList list, corto_event element);
CORTO_WEB_EXPORT corto_event corto_eventListTakeFirst(corto_eventList list);
CORTO_WEB_EXPORT corto_event corto_eventListLast(corto_eventList list);
CORTO_WEB_EXPORT void corto_eventListClear(corto_eventList list);
CORTO_WEB_EXPORT corto_event corto_eventListGet(corto_eventList list, corto_uint32 index);
CORTO_WEB_EXPORT corto_uint32 corto_eventListSize(corto_eventList list);

#ifdef __cplusplus
}
#endif
#endif

