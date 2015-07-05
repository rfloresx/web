# web
HTTP, WebSocket, SockJS and DDP server implementations for cortex

The web package contains functionality that allows for connecting cortex applications to the web. 

### HTTP server
_web::HTTPServer_

To be done

### WebSocket server
_web::WebSocketServer_

To be done

### SockJS server
_web::SockJsServer_

The SockJS server implements a subset (for now) of the [SockJS protocol](http://sockjs.github.io/sockjs-protocol/sockjs-protocol-0.3.3.html) that allows for setting up a WebSocket connection. 
This will allow applications to connect with JS clients that use SockJS as a library.

### DDP server
_web::DDPServer_

The DDP server is an implementation of the [DDP protocol](https://www.meteor.com/ddp) used by Meteor to synchronize data between a server and a client cache. 
The server maps DDP collections on Cortex scopes. This server enables writing Meteor apps with a Cortex backend.
The `examples/tasklist` example is a simple demonstration of Cortex & Meteor/DDP integration.
