/* web_DDPServer.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "web.h"

/* $header() */

/*
 * https://github.com/meteor/meteor/blob/devel/packages/ddp/DDP.md
 */

#include "mongoose.h"
#include "parson.h"

#define WEB_DDP_SERVER_SIZE_THRESHOLD 100
#define WEB_DDP_SERVER_BIG_LIST_SLEEP 10000000

// typedef void *web_DDPServer_ddpMsgHandler(web_DDPConnection conn, JSON_Object *json);

typedef cx_void web_DDPServer_ddpMsgHandler(web_DDPServer _this, web_DDPConnection conn, cx_word json);

/* $end */

/* ::cortex::web::DDPServer::connect(DDPConnection conn,word json) */
cx_void web_DDPServer_connect(web_DDPServer _this, web_DDPConnection conn, cx_word json) {
/* $begin(::cortex::web::DDPServer::connect) */
    CX_UNUSED(_this);
    JSON_Object *_json = (JSON_Object *)json;
    const char *session = json_object_get_string(_json, "session");
    const char *version = json_object_get_string(_json, "version");
    JSON_Array *support = json_object_get_array(_json, "support");
    CX_UNUSED(support);
    /* TODO Future support for sessions */
    if (session || (version && strcmp(version, "1"))) {
        goto failed;
    } else {
        web_DDPConnection_connected(conn);
    }
failed:;
    // DO NOTHING :/
    // web_DDPConnection_failed(conn);
/* $end */
}

/* ::cortex::web::DDPServer::onMessage(DDPConnection conn,string message) */
/* $header(::cortex::web::DDPServer::onMessage) */

cx_void web_DDPServer_forwardMessage(web_DDPServer _this, web_DDPConnection conn, const char *msg, JSON_Object *json) {
    static struct msgFunc { char *msg; web_DDPServer_ddpMsgHandler *handler; } functions[] = {
        {"connect", web_DDPServer_connect},
        {"ping", web_DDPServer_ping},
        {"pong", web_DDPServer_pong},
        {"sub", web_DDPServer_sub}
    };
    unsigned long int i;
    cx_bool notFoundMsg = TRUE;
    printf("size is :%lu\n", sizeof(functions));
    for (i = 0; i < sizeof(functions) && notFoundMsg; i++) {
        if (!strcmp(msg, functions[i].msg)) {
            json_object_remove(json, "msg"); /* for speed */
            functions[i].handler(_this, conn, (cx_word)json);
            notFoundMsg = FALSE;
        }
    }
    if (notFoundMsg) {
        cx_error("unknown message type: %s (length: %d)", msg, strlen(msg));
    }
}
/* $end */
cx_void web_DDPServer_onMessage(web_DDPServer _this, web_DDPConnection conn, cx_string message) {
/* $begin(::cortex::web::DDPServer::onMessage) */
    CX_UNUSED(_this);
    CX_UNUSED(conn);
    JSON_Value *msgJson = json_parse_string(message);
    if (!msgJson) {
        goto error;
    }
    if (json_value_get_type(msgJson) == JSONObject) {
        JSON_Object *jsonObj = json_value_get_object(msgJson);
        const char *msg = json_object_get_string(jsonObj, "msg");
        web_DDPServer_forwardMessage(_this, conn, msg, jsonObj);
    } else {
        printf("the json type was: %d\n", json_value_get_type(msgJson));
        web_DDPConnection_failed(conn);
    }
    json_value_free(msgJson);
error:;
/* $end */
}

/* ::cortex::web::DDPServer::ping(DDPConnection conn,word json) */
cx_void web_DDPServer_ping(web_DDPServer _this, web_DDPConnection conn, cx_word json) {
/* $begin(::cortex::web::DDPServer::ping) */
    CX_UNUSED(_this);
    JSON_Object *_json = (JSON_Object *)json;
    const char *id = json_object_get_string(_json, "id");
    web_DDPConnection_pong(conn, (cx_string)id);
/* $end */
}

/* ::cortex::web::DDPServer::pong(DDPConnection conn,word json) */
cx_void web_DDPServer_pong(web_DDPServer _this, web_DDPConnection conn, cx_word json) {
/* $begin(::cortex::web::DDPServer::pong) */
    CX_UNUSED(_this);
    JSON_Object *_json = (JSON_Object *)json;
    const char *id = json_object_get_string(_json, "id");
    if (conn->expectingPong) {
        if (conn->expectingPongId) {
            if (!id) {
                cx_error("Received pong without id %s on %s", conn->expectingPongId, cx_nameof(conn));
            }
            /* else, success */
            cx_dealloc(conn->expectingPongId);
            conn->expectingPongId = NULL;
        } else {
            if (id) {
                cx_error("Did not expect id %s on pong", id);
            }
            /* else, success */
        }
    } else {
        cx_error("Did not expect pong on connection %s", cx_nameof(conn));
    }
/* $end */
}

/* ::cortex::web::DDPServer::post(event e) */
/* $header(::cortex::web::DDPServer::post) */
static cx_observableEvent web_DDPServer_findRelatedEvent(web_DDPServer _this, cx_observableEvent e) {
    cx_iter iter = cx_llIter(_this->events);
    cx_observableEvent e2;
    while ((cx_iterHasNext(&iter))) {
        e2 = cx_iterNext(&iter);
        if ((e2->me == e->me) &&
          (e2->observable == e->observable) &&
          (e2->source == e->source) &&
          (e2->observer == e->observer)) {
            return e2;
        }
    }
    return NULL;
}
/* $end */
cx_void web_DDPServer_post(web_DDPServer _this, cx_event e) {
/* $begin(::cortex::web::DDPServer::post) */
    cx_uint32 size = 0;
    cx_observableEvent e2;

    cx_lock(_this);
    /* Check if there is already another event in the queue for the same object.
     * if so, replace event with latest update. */
    if ((e2 = web_DDPServer_findRelatedEvent(_this, cx_observableEvent(e)))) {
        cx_llReplace(_this->events, e2, e);
        cx_free(e2);
    } else {
        cx_llAppend(_this->events, e);
    }
    size = cx_llSize(_this->events);
    cx_unlock(_this);

    /* If queue is getting big, slow down publisher */
    if (size > WEB_DDP_SERVER_SIZE_THRESHOLD) {
        cx_sleep(0, WEB_DDP_SERVER_BIG_LIST_SLEEP);
    }
/* $end */
}

/* ::cortex::web::DDPServer::step() */
cx_bool web_DDPServer_step(web_DDPServer _this) {
/* $begin(::cortex::web::DDPServer::step) */
    cx_event e;
    cx_ll events = cx_llNew();
    cx_bool result = TRUE;
    web_WebSocketServer __this = web_WebSocketServer(_this);

    web_WebSocketServer_poll(__this);
    if (__this->exiting) {
        result = FALSE;
    }
    
    struct mg_server *server = (struct mg_server *)__this->server;
    cx_lock(_this);
    while ((e = cx_llTakeFirst(_this->events))) {
        cx_llAppend(events, e);
    }
    cx_unlock(_this);
    while ((e = cx_llTakeFirst(events))) {
        cx_event_handle(e);
        cx_free(e);
        mg_poll_server(server, 1);
    }
    
    return result;
/* $end */
}

/* ::cortex::web::DDPServer::sub(DDPConnection conn,word json) */
cx_void web_DDPServer_sub(web_DDPServer _this, web_DDPConnection conn, cx_word json) {
/* $begin(::cortex::web::DDPServer::sub) */
    puts("sub!!!");
    CX_UNUSED(_this);
    CX_UNUSED(conn);
    CX_UNUSED(json);
/* $end */
}

/* ::cortex::web::DDPServer::unsub(DDPConnection conn,word json) */
cx_void web_DDPServer_unsub(web_DDPServer _this, web_DDPConnection conn, cx_word json) {
/* $begin(::cortex::web::DDPServer::unsub) */
    puts("unsub!!!");
    CX_UNUSED(_this);
    CX_UNUSED(conn);
    CX_UNUSED(json);
/* $end */
}
