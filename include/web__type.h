/* web__type.h
 *
 * Type definitions for C-language.
 * This file contains generated code. Do not modify!
 */

#ifndef corto_web__type_H
#define corto_web__type_H

#include "corto.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Casting macro's for classes */
#define web_DDPServer(o) ((web_DDPServer)corto_assertType((corto_type)web_DDPServer_o, o))
#define web_DDPServer_Collection(o) ((web_DDPServer_Collection)corto_assertType((corto_type)web_DDPServer_Collection_o, o))
#define web_DDPServer_Publication(o) ((web_DDPServer_Publication)corto_assertType((corto_type)web_DDPServer_Publication_o, o))
#define web_DDPServer_readyEvent(o) ((web_DDPServer_readyEvent)corto_assertType((corto_type)web_DDPServer_readyEvent_o, o))
#define web_DDPServer_Session(o) ((web_DDPServer_Session)corto_assertType((corto_type)web_DDPServer_Session_o, o))
#define web_DDPServer_Subscription(o) ((web_DDPServer_Subscription)corto_assertType((corto_type)web_DDPServer_Subscription_o, o))
#define web_handler(o) ((web_handler *)corto_assertType((corto_type)web_handler_o, o))
#define web_messageHandler(o) ((web_messageHandler *)corto_assertType((corto_type)web_messageHandler_o, o))
#define web_SockJsServer(o) ((web_SockJsServer)corto_assertType((corto_type)web_SockJsServer_o, o))
#define web_SockJsServer_Connection(o) ((web_SockJsServer_Connection)corto_assertType((corto_type)web_SockJsServer_Connection_o, o))
#define web_SockJsServer_handler(o) ((web_SockJsServer_handler *)corto_assertType((corto_type)web_SockJsServer_handler_o, o))
#define web_SockJsServer_messageHandler(o) ((web_SockJsServer_messageHandler *)corto_assertType((corto_type)web_SockJsServer_messageHandler_o, o))
#define web_SockJsServer_uriHandler(o) ((web_SockJsServer_uriHandler *)corto_assertType((corto_type)web_SockJsServer_uriHandler_o, o))
#define web_SockJsServer_UriRequest(o) ((web_SockJsServer_UriRequest *)corto_assertType((corto_type)web_SockJsServer_UriRequest_o, o))
#define web_WebSocketConnection(o) ((web_WebSocketConnection)corto_assertType((corto_type)web_WebSocketConnection_o, o))
#define web_WebSocketServer(o) ((web_WebSocketServer)corto_assertType((corto_type)web_WebSocketServer_o, o))

/* Type definitions */
/*  /corto/web/SockJsServer/messageHandler */
typedef struct web_SockJsServer_messageHandler web_SockJsServer_messageHandler;

struct web_SockJsServer_messageHandler {
    corto_delegatedata _parent;
};

/*  /corto/web/SockJsServer/handler */
typedef struct web_SockJsServer_handler web_SockJsServer_handler;

struct web_SockJsServer_handler {
    corto_delegatedata _parent;
};

/*  /corto/web/SockJsServer/uriHandler */
typedef struct web_SockJsServer_uriHandler web_SockJsServer_uriHandler;

struct web_SockJsServer_uriHandler {
    corto_delegatedata _parent;
};

/*  /corto/web/SockJsServer */
CORTO_CLASS(web_SockJsServer);

CORTO_CLASS_DEF(web_SockJsServer) {
    corto_uint16 port;
    web_SockJsServer_messageHandler onMessage;
    web_SockJsServer_handler onOpen;
    web_SockJsServer_handler onClose;
    web_SockJsServer_uriHandler onUri;
    corto_uint16 pollTimemoutMillis;
    corto_word thread;
    corto_bool exiting;
    corto_word server;
    corto_uint32 timeElapsed;
    corto_object connections;
};

CORTO_LIST(corto_eventList);

/*  /corto/web/DDPServer */
CORTO_CLASS(web_DDPServer);

CORTO_CLASS_DEF(web_DDPServer) {
    CORTO_EXTEND(web_SockJsServer);
    corto_bool autoPublish;
    corto_eventList events;
    corto_object sessions;
};

/*  /corto/web/DDPServer/Collection */
CORTO_CLASS(web_DDPServer_Collection);

CORTO_CLASS_DEF(web_DDPServer_Collection) {
    corto_object obj;
    corto_bool value;
    corto_bool meta;
    corto_bool scope;
};

/*  /corto/web/DDPServer/Publication */
CORTO_CLASS(web_DDPServer_Publication);

CORTO_CLASS_DEF(web_DDPServer_Publication) {
    corto_object scope;
};

/*  /corto/web/DDPServer/readyEvent */
CORTO_CLASS(web_DDPServer_readyEvent);

CORTO_CLASS_DEF(web_DDPServer_readyEvent) {
    CORTO_EXTEND(corto_event);
};

/*  /corto/web/SockJsServer/Connection */
CORTO_CLASS(web_SockJsServer_Connection);

CORTO_CLASS_DEF(web_SockJsServer_Connection) {
    corto_object data;
    corto_word conn;
};

/*  /corto/web/DDPServer/Session */
CORTO_CLASS(web_DDPServer_Session);

CORTO_CLASS_DEF(web_DDPServer_Session) {
    web_SockJsServer_Connection conn;
    corto_object collections;
    corto_object subscriptions;
};

/*  /corto/web/DDPServer/Subscription */
CORTO_CLASS(web_DDPServer_Subscription);

CORTO_CLASS_DEF(web_DDPServer_Subscription) {
    web_DDPServer_Publication pub;
    corto_string id;
    corto_bool value;
    corto_bool meta;
    corto_bool scope;
};

/*  /corto/web/handler */
typedef struct web_handler web_handler;

struct web_handler {
    corto_delegatedata _parent;
};

/*  /corto/web/messageHandler */
typedef struct web_messageHandler web_messageHandler;

struct web_messageHandler {
    corto_delegatedata _parent;
};

/*  /corto/web/SockJsServer/UriRequest */
typedef struct web_SockJsServer_UriRequest web_SockJsServer_UriRequest;

struct web_SockJsServer_UriRequest {
    corto_word conn;
};

/*  /corto/web/WebSocketConnection */
CORTO_CLASS(web_WebSocketConnection);

CORTO_CLASS_DEF(web_WebSocketConnection) {
    corto_object data;
    corto_word conn;
};

/*  /corto/web/WebSocketServer */
CORTO_CLASS(web_WebSocketServer);

CORTO_CLASS_DEF(web_WebSocketServer) {
    corto_uint16 port;
    web_handler onOpen;
    web_handler onClose;
    web_messageHandler onMessage;
    corto_uint16 pollTimemoutMillis;
    corto_word thread;
    corto_bool exiting;
    corto_word server;
};

#ifdef __cplusplus
}
#endif
#endif

