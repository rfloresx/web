# web
HTTP, WebSocket, SockJS and DDP server implementations for cortex

The web package contains functionality that allows for connecting cortex applications to the web. 

## Building web
Web requires [cortex](http://www.github.com/cortexlang/cortex) to be installed on your machine. Before building web, please ensure that cortex is available in your environment (you can quickly check by typing `cxsh`). If not, run `source configure` from the cortex installation folder.

When cortex is succesfully installed, go to the `web` installation folder. Run the following command:
```
rake
```
After rake has finished, a library called `libweb.so` will now be located in <web install dir>/bin. To use it, run the following command:
```
cxsh ./bin/libweb.so
```
That will start the cortex shell and load the web library. In the cortex shell, you can now type:
```
web::DDPServer s: 8000
```
This will start a DDP server on port 8000. Enjoy!

## Contents
### HTTP server
_::cortex::web::HTTPServer_

To be done

### WebSocket server
_::cortex::web::WebSocketServer_

To be done

### SockJS server
_::cortex::web::SockJsServer_

The SockJS server implements a subset (for now) of the [SockJS protocol](http://sockjs.github.io/sockjs-protocol/sockjs-protocol-0.3.3.html) that allows for setting up a WebSocket connection. 
This will allow applications to connect with JS clients that use SockJS as a library.

### DDP server
_::cortex::web::DDPServer_

The DDP server is an implementation of the [DDP protocol](https://www.meteor.com/ddp) used by Meteor to synchronize data between a server and a client cache. 
The server maps DDP collections on Cortex scopes. This server enables writing Meteor apps with a Cortex backend.
The `examples/tasklist` example is a simple demonstration of Cortex & Meteor/DDP integration.
