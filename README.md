# /corto/web
### random(uint16 n)
#### n
#### Returns

## DDP
### autoPublish
### Collection
### events
### getPublication(string name)
#### name
#### Returns
### onData(/corto/web/HTTP/Connection c,string msg)
#### c
#### msg
### onPoll()
### post(event e)
#### e
### Publication
### readyEvent
### Session
### sessions
### Subscription

## DDP/Collection
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
### failed(/corto/web/HTTP/Connection conn)
#### conn
### getCollection(string name)
#### name
#### Returns
### getSub(/corto/web/DDP/Publication pub,string id,bool meta,bool value,bool scope)
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

## DDP/Subscription
### id
### meta
### pub
### ready()
### scope
### value

## HTTP
### addService(/corto/web/Service s)
#### s
### broadcast(string msg)
#### msg
### Connection
### connections
### destruct()
### doClose(/corto/web/HTTP/Connection c)
#### c
### doMessage(/corto/web/HTTP/Connection c,string msg)
#### c
#### msg
### doOpen(/corto/web/HTTP/Connection c)
#### c
### doPoll()
### doRequest(/corto/web/HTTP/Connection c,/corto/web/HTTP/Request r)
#### c
#### r
### get(uint16 port)
#### port
#### Returns
### pollInterval
### port
### removeService(/corto/web/Service s)
#### s
### Request
### services
### set(uint16 port,/corto/web/HTTP server)
#### port
#### server
#### Returns
### write(/corto/web/HTTP/Connection c,string msg)
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

## REST
### construct()
#### Returns
### onRequest(/corto/web/HTTP/Connection c,/corto/web/HTTP/Request r)
#### c
#### r
#### Returns
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
### onPoll()
### onRequest(/corto/web/HTTP/Connection c,/corto/web/HTTP/Request r)
#### c
#### r
#### Returns
### port
### server

## SockJs
### construct()
#### Returns
### onClose(/corto/web/HTTP/Connection c)
#### c
### onData(/corto/web/HTTP/Connection c,string msg)
#### c
#### msg
### onMessage(/corto/web/HTTP/Connection c,string msg)
#### c
#### msg
### onOpen(/corto/web/HTTP/Connection c)
#### c
### onPoll()
### onRequest(/corto/web/HTTP/Connection c,/corto/web/HTTP/Request r)
#### c
#### r
#### Returns
### timeElapsed

## StandaloneHTTP
### construct()
#### Returns
### destruct()
### exiting
### server
### thread
### write(/corto/web/HTTP/Connection c,string msg)
#### c
#### msg
