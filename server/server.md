# /corto/web/server
### random(uint16 n)
#### n
#### Returns

## DDP
### autoPublish
### Collection
### construct()
#### Returns
### events
### getPublication(string name)
#### name
#### Returns
### init()
#### Returns
### onClose(/corto/web/server/HTTP/Connection c)
#### c
### onData(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg
### onPoll()
### post(/corto/core/event e)
#### e
### Publication
### purge(object observable)
#### observable
### readyEvent
### Session
### sessions
### Subscription

## DDP/Collection
### construct()
#### Returns
### destruct()
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
### unsubscribe(bool value,bool meta,bool scope)
#### value
#### meta
#### scope
### value

## DDP/Publication
### create(object session,object sub)
#### session
#### sub
#### Returns
### scope

## DDP/readyEvent

## DDP/Session
### collections
### conn
### connected()
### construct()
#### Returns
### error(string reason,string offendingMessage)
#### reason
#### offendingMessage
### failed(/corto/web/server/HTTP/Connection conn)
#### conn
### getCollection(string name,bool meta,bool value,bool scope)
#### name
#### meta
#### value
#### scope
#### Returns
### getSub(/corto/web/server/DDP/Publication pub,string id,bool meta,bool value,bool scope)
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
### unsub(string id)
#### id

## DDP/Subscription
### id
### meta
### pub
### ready()
### scope
### value

## Files
### construct()
#### Returns
### onRequest(/corto/web/server/HTTP/Connection c,/corto/web/server/HTTP/Request r,string uri)
#### c
#### r
#### uri
#### Returns
### path
### prefix

## HTTP
### addService(/corto/web/server/Service s)
#### s
### broadcast(string msg)
#### msg
### Connection
### connections
### destruct()
### doClose(/corto/web/server/HTTP/Connection c)
#### c
### doMessage(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg
### doOpen(/corto/web/server/HTTP/Connection c)
#### c
### doPoll()
### doRequest(/corto/web/server/HTTP/Connection c,/corto/web/server/HTTP/Request r)
#### c
#### r
### get(uint16 port)
#### port
#### Returns
### pollInterval
### port
### removeService(/corto/web/server/Service s)
#### s
### Request
### services
### set(uint16 port,/corto/web/server/HTTP server)
#### port
#### server
#### Returns
### write(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg

## HTTP/Connection
### conn
### server
### udata
### write(string msg)
#### msg

## HTTP/Request
### conn
### file
### getVar(string id)
#### id
#### Returns
### reply(string msg)
#### msg
### sendfile(string file)
#### file
### setHeader(string name,string val)
#### name
#### val
### setStatus(uint16 status)
#### status
### uri

## REST
### construct()
#### Returns
### onRequest(/corto/web/server/HTTP/Connection c,/corto/web/server/HTTP/Request r,string uri)
#### c
#### r
#### uri
#### Returns
### prefix

## Service
### construct()
#### Returns
### onClose(/corto/web/server/HTTP/Connection c)
#### c
### onMessage(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg
### onOpen(/corto/web/server/HTTP/Connection c)
#### c
### onPoll()
### onRequest(/corto/web/server/HTTP/Connection c,/corto/web/server/HTTP/Request r,string uri)
#### c
#### r
#### uri
#### Returns
### port
### prefix
### server

## SockJs
### construct()
#### Returns
### onClose(/corto/web/server/HTTP/Connection c)
#### c
### onData(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg
### onMessage(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg
### onOpen(/corto/web/server/HTTP/Connection c)
#### c
### onPoll()
### onRequest(/corto/web/server/HTTP/Connection c,/corto/web/server/HTTP/Request r,string uri)
#### c
#### r
#### uri
#### Returns
### timeElapsed
### write(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg

## StandaloneHTTP
### construct()
#### Returns
### destruct()
### exiting
### server
### thread
### write(/corto/web/server/HTTP/Connection c,string msg)
#### c
#### msg
