# web
Web functionality for Cortex. 

With the `web` package you can build interactive web applications on top of cortex platform. It contains a `web::server` class that exposes a REST based API and will be extended with websocket functionality. The server works best in conjunction with the web application in the `interface/web` folder of the http://github.com/SeldomBerry/cortex project.

The server package uses the Mongoose HTTP server (http://github.com/cesanta/mongoose). 

### Getting started
Clone the repository in a folder called `web`. Make sure that the `cortex` repository is built and the cortex tools are available from the environment (see http://github.com/SeldomBerry/cortex). Build the rest package with `make -C rest`.

To start a server using the default Cortex web application, execute the following commands:
```
cd $CORTEX_HOME/interface/web
cxsh
```
Then from the shell, execute the following statement:
```
rest::server s: 8000
```
The web server is now running. Verify by going to `http://localhost:8000` which will display the Cortex web interface. A way of directly invoking the REST api directly is with the `curl` command:
```
curl localhost:8000/cortex/lang/class
```
The server will reply with the JSON code generated from `::cortex::lang::class`.
