/* rest_server.c
 *
 * This file contains the implementation for the generated interface.
 *
 *    Don't mess with the begin and end tags, since these will ensure that modified
 *    code in interface functions isn't replaced when code is re-generated.
 */

#include "rest.h"
#include "rest__meta.h"

/* $header() */
#include "mongoose.h"
#include "json.h"

static const char *s_no_cache_header =
  "Cache-Control: max-age=0, post-check=0, "
  "pre-check=0, no-store, no-cache, must-revalidate\r\n";

static int rest_deliverFile(struct mg_connection *conn, const char *file) {
    mg_send_file(conn, file, s_no_cache_header);
    return MG_MORE; /* Indicate that page will request more data */    
}

static int rest_printParents(struct mg_connection *conn, cx_object o) {
    int result = 0;
    struct cx_serializer_s serializer = cx_json_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    
    if (o) {
        cx_json_ser_t jsonData = {NULL, NULL, 0, 0, 0, TRUE, FALSE, FALSE, TRUE};
        if (cx_parentof(o)) {
            result += rest_printParents(conn, cx_parentof(o));
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

static int rest_deliverResource(struct mg_connection *conn) {
    cx_object o = NULL;

    /* Resolve object based on URI */
    if (strcmp(conn->uri, "/::")) {
        o = cx_resolve(NULL, (cx_string)conn->uri);
    } else {
        o = root_o;
        cx_keep(o);
    }

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
                if (rest_printParents(conn, cx_parentof(o))) {
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

static int rest_handler(struct mg_connection *conn, enum mg_event ev) {
    int result = MG_TRUE;

    switch (ev) {
    case MG_AUTH:
        break;
    case MG_REQUEST: {

        if (*conn->uri && !(conn->uri[1])) {
            /* Empty URI, forward index.html */
            result = rest_deliverFile(conn, "index.html");
        } else if (strchr (conn->uri, '.')) {
            result = rest_deliverFile(conn, conn->uri + 1);
        } else {
            result = rest_deliverResource(conn);
        }

        break;
    }
    default: 
        result = MG_FALSE;
        break;
    }

    return result;
}

/* Run the server in a separate thread */
void* rest_run(void *data) {
    struct mg_server *server;
    rest_server _this = data;
    char portStr[6];
    sprintf(portStr, "%u", _this->port);

    // Create and configure the server
    server = mg_create_server(NULL, rest_handler);
    mg_set_option(server, "listening_port", portStr);

    // Serve request. Hit Ctrl-C to terminate the program
    for (;;) {
        mg_poll_server(server, 1000);
    }

    // Cleanup, and free server instance
    mg_destroy_server(&server);
}

/* $end */

/* ::cortex::rest::server::construct() */
cx_int16 rest_server_construct(rest_server _this) {
/* $begin(::cortex::rest::server::construct) */
    _this->thread = (cx_word)cx_threadNew(rest_run, _this);
    if (!_this->thread) {
        goto error;
    }
    return 0;
error:
    return -1;
/* $end */
}

/* ::cortex::rest::server::destruct() */
cx_void rest_server_destruct(rest_server _this) {
/* $begin(::cortex::rest::server::destruct) */
    CX_UNUSED(_this);
/* $end */
}
