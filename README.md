# web
Web packages for Cortex. 

At this moment web contains a `rest` package, which is a REST API for accessing the Cortex object store. In the future, this API will be extended with the capability to remotely modify objects. Additionally a package containing a websocket-based protocol will be added.

The `rest` package uses the Mongoose HTTP server (http://github.com/cesanta/mongoose). 

### Getting started
Clone the repository in a folder called `web`. Make sure that the `cortex` repository is built and the cortex tools are available from the environment (see http://github.com/SeldomBerry/cortex). Build the web package with `make -C web`.

To start a server using the default Cortex web application, run the following commands:
```
cd $(CORTEX_HOME)/interface/web
cxsh
```
Then from the shell, execute the following statement:
```
rest::server s: 8000
```
The web server is now running. You can verify with the following address in your webbrowser: `http://localhost:8000` which will display the Cortex web user interface. Another way of invoking the REST api directly is with the `curl` command:
```
curl localhost:800/cortex/lang/class
```
This will return the JSON generated from the `::cortex::lang::class` object.
