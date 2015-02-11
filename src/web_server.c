/* web_server.c
 *
 * This file contains the implementation for the generated interface.
 *
 *    Don't mess with the begin and end tags, since these will ensure that modified
 *    code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"
#include "web__meta.h"

/* $header() */
#include "mongoose.h"
#include "json.h"

/* Serve a file (html, icon, css, js) */
static int web_serveFile(struct mg_connection *conn, const char *file) {
    mg_send_file(conn, file, "");
    return MG_MORE; /* Indicate that page will request more data */    
}

/* Write all parents to connection so that a reply is self-containing */
static int web_printParents(struct mg_connection *conn, cx_object o) {
    int result = 0;
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    
    if (o) {
        cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, TRUE, FALSE, FALSE, TRUE};
        if (cx_parentof(o)) {
            result += web_printParents(conn, cx_parentof(o));
        }

        if (result) {
            mg_printf_data(conn, ",");
        }

        cx_serialize(&serializer, o, &jsonData);
        mg_printf_data(conn, "%s\n", jsonData.buffer);
        cx_dealloc(jsonData.buffer);

        result++;
    }

    return result;
}

/* Serve a Cortex object */
static int web_serveObject(struct mg_connection *conn) {
    cx_object o = NULL;

    /* Resolve object based on URI */
    o = cx_resolve(NULL, (cx_string)conn->uri + 2);

    if (!o) {
        mg_send_status(conn, 404);
        mg_printf_data(conn, "404: resource '%s' not found\n", conn->uri);
    } else {
        char option[6];
        cx_bool value = 0;
        cx_bool meta = 0;
        cx_bool scope = 0;

        /* Set correct content type */
        mg_send_header(conn, "Content-Type", "application/json; charset=utf-8");

        /* Determine what to show */
        mg_get_var(conn, "value", option, sizeof(option));
        if (!strcmp(option, "true")) { value = TRUE; }

        mg_get_var(conn, "meta", option, sizeof(option));
        if (!strcmp(option, "true")) { meta = TRUE; }

        mg_get_var(conn, "scope", option, sizeof(option));
        if (!strcmp(option, "true")) { scope = TRUE; }

        if (!(value || meta || scope)) {
            value = TRUE;
        }

        {
            /* Serialize object-value to JSON */
            struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);

            if ((cx_typeof(o)->kind == CX_VOID) && (!meta && !scope)) {
                mg_printf_data(conn, "\n");
            } else {
                mg_printf_data(conn, "[");

                /* Serialize metadata of parents */
                if (web_printParents(conn, cx_parentof(o))) {
                    mg_printf_data(conn, ",");
                }

                /* Serialize value */              
                {
                    cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, meta, value, scope, TRUE};
                    cx_serialize(&serializer, o, &jsonData);
                    mg_printf_data(conn, "%s\n", jsonData.buffer);
                    cx_dealloc(jsonData.buffer);
                }
                mg_printf_data(conn, "]\n");
            }
        }

        cx_free(o);
    }

    return MG_TRUE;
}

/* Handle subscribe message */
static int web_subscribe(struct mg_connection *conn) {
    char *flag_start = conn->content + 2;
    char *id_start = strchr(flag_start, ' ') + 1;
    int len;
    cx_id id;
    cx_object o = NULL; 

    /* Check if message is correct */
    if ((id_start - conn->content) > (int)conn->content_len) {
        printf("error: message '%.*s' not correctly formatted\n",
            (int)conn->content_len, conn->content);
        goto error;
    }

    /* Copy identifier to buffer */
    len = (int)conn->content_len - (id_start - conn->content);
    memcpy(id, id_start, len);
    id[len] = '\0';

    /* Silence previous subscription */
    if (web_wsdata(conn->connection_param)->observable) {
        cx_silence(web_wsdata(conn->connection_param)->observable, 
            web_wsdata_trigger_o, web_wsdata(conn->connection_param));
    }
    web_wsdata(conn->connection_param)->eventCount = 0;

    /* Lookup object */
    o = cx_resolve(NULL, id);
    if (!o) {
        goto error;
    } else if (!cx_checkAttr(o, CX_ATTR_OBSERVABLE)) {
        /* Can't listen if object is not observable, but can send the client the value */
        cx_set(&web_wsdata(conn->connection_param)->observable, NULL);
        web_wsdata_send(web_wsdata(conn->connection_param), o, TRUE, TRUE, TRUE, TRUE);
        web_wsdata(conn->connection_param)->eventCount++;
    } else {
        /* Configure observer to listen for updates */
        cx_set(&web_wsdata(conn->connection_param)->observable, o);
        cx_listen(o, web_wsdata_trigger_o, web_wsdata(conn->connection_param));
    }

    return 0;
error:
    return -1;
}

/* Set throttle value on connection */
static void web_throttle(struct mg_connection *conn) {
    web_wsdata(conn->connection_param)->clientReceived = atoi(conn->content + 2);
}

/* Handle websocket message */
static int web_socketHandler(struct mg_connection *conn) {
    switch (*conn->content) {
    case 'L':
        web_subscribe(conn);
        break;
    case 'T':
        web_throttle(conn);
        break;
    default:
        break;
    }
    return MG_TRUE;
}

/* Handle incoming HTTP events */
static int web_handler(struct mg_connection *conn, enum mg_event ev) {
    int result = MG_TRUE;

    switch (ev) {
    case MG_AUTH:
        break;

    /* New websocket connection */
    case MG_WS_CONNECT:
        conn->connection_param = web_wsdata__create((cx_word)conn, NULL);
        mg_websocket_printf(conn, WEBSOCKET_OPCODE_TEXT, "");
        result = MG_FALSE;
        break;

    /* Close websocket connection */
    case MG_CLOSE:
        if (conn->connection_param) {
            cx_free(conn->connection_param);
            conn->connection_param = NULL;
        }
        break;

    /* HTTP request */
    case MG_REQUEST: 
        /* If request is a websocket, forward to websocket handler */
        if (conn->is_websocket) {
            if (conn->content_len) {
                result = web_socketHandler(conn);
            }
        } else {
            /* If the URI is prefixed with a '_' an object is requested */
            if (!memcmp(conn->uri, "/_", 2)) {
                result = web_serveObject(conn);

            /* If the URI contains a '.' before a '?' a file is requested */
            } else if (strchr (conn->uri, '.')) {
                result = web_serveFile(conn, conn->uri + 1);

            /* If none of the above, serve index.html. Since the client
             * is a single page app, the URI can contain a path to the
             * object currently displayed. This URI is not relevant for
             * the server. However, when such a URI is requested, the 
             * index.html file must be served. */
            } else {
                result = web_serveFile(conn, "index.html");
            }
        }

        break;
    default: 
        result = MG_FALSE;
        break;
    }

    return result;
}

/* Run the server in a separate thread */
void* web_run(void *data) {
    struct mg_server *server;
    web_server _this = data;
    char portStr[6];
    sprintf(portStr, "%u", _this->port);

    // Create and configure the server
    server = mg_create_server(NULL, web_handler);
    mg_set_option(server, "listening_port", portStr);

    // Serve request. Hit Ctrl-C to terminate the program
    for (;;) {
        mg_poll_server(server, 1000);
    }

    // Cleanup, and free server instance
    mg_destroy_server(&server);
}
/* $end */

/* ::cortex::web::server::construct() */
cx_int16 web_server_construct(web_server _this) {
/* $begin(::cortex::web::server::construct) */
    _this->thread = (cx_word)cx_threadNew(web_run, _this);
    if (!_this->thread) {
        goto error;
    }
    return 0;
error:
    return -1;
/* $end */
}

/* ::cortex::web::server::destruct() */
cx_void web_server_destruct(web_server _this) {
/* $begin(::cortex::web::server::destruct) */
    CX_UNUSED(_this);
/* $end */
}
