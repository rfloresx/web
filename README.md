# /corto/web
### random(uint16 n)
#### n
#### Returns

## DDPServer
### autoPublish
### Collection
### construct()
#### Returns
### events
### getPublication(string name)
#### name
#### Returns
### onClose(/corto/web/SockJsServer/Connection conn)
#### conn
### onMessage(/corto/web/SockJsServer/Connection conn,string message)
#### conn
#### message
### onUri(/corto/web/SockJsServer/UriRequest conn,string uri)
#### conn
#### uri
### post(event e)
#### e
### Publication
### readyEvent
### run()
### Session
### sessions
### Subscription

## DDPServer/Collection
### construct()
#### Returns
### meta
### obj
### onAdded
#### observable
### onChanged
#### observable
### onRemoved
#### observable
### scope
### subscribe(bool value,bool meta,bool scope)
#### value
#### meta
#### scope
### value

## DDPServer/Publication
### create(object session,object sub)
#### session
#### sub
#### Returns
### scope

## DDPServer/readyEvent

## DDPServer/Session
### collections
### conn
### connected()
### construct()
#### Returns
### error(string reason,string offendingMessage)
#### reason
#### offendingMessage
### failed(/corto/web/SockJsServer/Connection conn)
#### conn
### getCollection(string name)
#### name
#### Returns
### getSub(/corto/web/DDPServer/Publication pub,string id,bool meta,bool value,bool scope)
#### pub
#### id
#### meta
#### value
#### scope
#### Returns
### pong(string id)
#### id
### sub(string id,string name,bool meta,bool value,bool scope)
#### id
#### name
#### meta
#### value
#### scope
### subscriptions

## DDPServer/Subscription
### id
### meta
### pub
### ready()
### scope
### value

## handler

## HTTP
### addService(/corto/web/Service s)
#### s
### Connection
### destruct()
### doClose(/corto/web/HTTP/Connection c)
#### c
### doMessage(/corto/web/HTTP/Connection c,string msg)
#### c
#### msg
### doOpen(/corto/web/HTTP/Connection c)
#### c
### doRequest(/corto/web/HTTP/Connection c,/corto/web/HTTP/Request r)
#### c
#### r
### get(uint16 port)
#### port
#### Returns
### port
### removeService(/corto/web/Service s)
#### s
### Request
### services
### set(uint16 port,/corto/web/HTTP server)
#### port
#### server
#### Returns

## HTTP/Connection
### conn
### udata

## HTTP/Request
### conn
### getVar(string id)
#### id
#### Returns
### reply(string msg)
#### msg
### setHeader(string name,string val)
#### name
#### val
### setStatus(uint16 status)
#### status
### uri

## messageHandler

## REST
### construct()
#### Returns
### onRequest(/corto/web/HTTP/Connection c,/corto/web/HTTP/Request r)
#### c
#### r
### prefix

## Service
### construct()
#### Returns
### onClose(/corto/web/HTTP/Connection c)
#### c
### onMessage(/corto/web/HTTP/Connection c,string msg)
#### c
#### msg
### onOpen(/corto/web/HTTP/Connection c)
#### c
### onRequest(/corto/web/HTTP/Connection c,/corto/web/HTTP/Request r)
#### c
#### r
### port
### server

## SockJsServer
### Connection
### connections
### construct()
#### Returns
### destruct()
### exiting
### handler
### messageHandler
### onClose
### onMessage
### onOpen
### onUri
### poll(uint32 msec)
#### msec
### pollTimemoutMillis
### port
### run()
### server
### thread
### timeElapsed
### uriHandler
### UriRequest

## SockJsServer/Connection
### conn
### data
### send(string msg)
#### msg

## SockJsServer/handler

## SockJsServer/messageHandler

## SockJsServer/uriHandler

## SockJsServer/UriRequest
### conn
### getVar(string id)
#### id
#### Returns
### setHeader(string name,string val)
#### name
#### val
### setStatus(uint16 status)
#### status
### write(string msg)
#### msg

## StandaloneHTTP
### construct()
#### Returns
### destruct()
### exiting
### pollInterval
### server
### thread

## WebSocketConnection
### conn
### data
### send(string message)
#### message

## WebSocketServer
### construct()
#### Returns
### destruct()
### exiting
### onClose
### onMessage
### onOpen
### poll()
### pollTimemoutMillis
### port
### run()
### server
### thread
