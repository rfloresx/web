[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/cortolang/web?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=body_badge)

HTTP, WebSocket, SockJS and DDP server implementations for corto

## Building web
Web requires [corto](http://www.github.com/cortolang/corto) to be installed on your machine. Before building web, please ensure that corto is available in your environment (you can quickly check by typing `cxsh`). If not, run `source configure` from the corto installation folder.

When corto is succesfully installed, go to the `web` installation folder. Run the following command:
```
rake
```
After rake has finished, a library called `libweb.so` will now be located in <web install dir>/bin. To use it, run the following command:
```
cxsh ./bin/libweb.so
```
That will start the corto shell and load the web library. In the corto shell, you can now type:
```
web::DDPServer s: 8000
```
This will start a DDP server on port 8000. Also, check out our [DDP example](https://github.com/cortolang/web/tree/master/examples/tasklist)! Enjoy!

## Contents
### HTTP server
_::corto::web::HTTPServer_

To be done

### WebSocket server
_::corto::web::WebSocketServer_

To be done

### SockJS server
_::corto::web::SockJsServer_

The SockJS server implements a subset (for now) of the [SockJS protocol](http://sockjs.github.io/sockjs-protocol/sockjs-protocol-0.3.3.html) that allows for setting up a WebSocket connection. 
This will allow applications to connect with JS clients that use SockJS as a library.

### DDP server
_::corto::web::DDPServer_

The DDP server is an implementation of the [DDP protocol](https://www.meteor.com/ddp) used by Meteor to synchronize data between a server and a client cache. 
The server maps DDP collections on Corto scopes. This server enables writing Meteor apps with a Corto backend.
The `examples/tasklist` example is a simple demonstration of Corto & Meteor/DDP integration.
