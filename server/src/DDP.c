/* $CORTO_GENERATED
 *
 * DDP.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */
#include "corto/fmt/json/json.h"

#define SERVER_DDP_SERVER_SIZE_THRESHOLD 100
#define SERVER_DDP_SERVER_BIG_LIST_SLEEP 10000000

static corto_void server_DDP_connect(server_DDP this, server_HTTP_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *session = json_object_get_string(json, "session");
    const char *version = json_object_get_string(json, "version");
    server_DDP_Session ddpSession = NULL;

    /* TODO Future support for sessions */
    if (version && strcmp(version, "1")) {
        goto failed;
    } else {
        if (!session || !(ddpSession = corto_resolve(this->sessions, (corto_string)session))) {
            char *sessionId = server_random(17);
            ddpSession = server_DDP_SessionCreateChild(this->sessions, sessionId, conn);
            corto_setref(&conn->udata, ddpSession);
            corto_dealloc(sessionId);
        } else {
            corto_setref(&ddpSession->conn, conn);
            corto_setref(&conn->udata, ddpSession);
            corto_release(ddpSession);
        }

        corto_trace("[ddp] session %s: new session", corto_idof(ddpSession));
        server_DDP_Session_connected(ddpSession);
    }

    return;
failed:
    corto_trace("[ddp] connection attempt failed");
    server_DDP_Session_failed(conn);
}

static corto_void server_DDP_ping(server_DDP this, server_HTTP_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *id = json_object_get_string(json, "id");
    server_DDP_Session_pong(conn->udata, (corto_string)id);
}

static corto_void server_DDP_sub(server_DDP this, server_HTTP_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *id = json_object_get_string(json, "id");
    const char *name = json_object_get_string(json, "name");
    JSON_Array *params = json_object_get_array(json, "params");
    JSON_Object *cxparams = json_array_get_object(params, 0);
    corto_bool meta = FALSE, value = FALSE, scope = FALSE;
    int v;

    if ((v = json_object_get_boolean(cxparams, "meta")) != -1) {
        meta = v;
    }
    if ((v = json_object_get_boolean(cxparams, "scope")) != -1) {
        scope = v;
    }
    if ((v = json_object_get_boolean(cxparams, "value")) != -1) {
        value = v;
    }

    corto_trace("[ddp] session %s: subscribe for %s (subscription id = %s, value = %d)",
        corto_idof(conn->udata),
        name,
        id,
        value);

    server_DDP_Session_sub(conn->udata, (corto_string)id, (corto_string)name, meta, value, scope);
}

static corto_void server_DDP_unsub(server_DDP this, server_HTTP_Connection conn, JSON_Object *json) {
    CORTO_UNUSED(this);
    const char *id = json_object_get_string(json, "id");

    corto_trace("[ddp] session %s: unsubscribe subscription %s",
        corto_idof(conn->udata),
        id);

    server_DDP_Session_unsub(conn->udata, (corto_string)id);
}

static corto_void server_DDP_callMethod(server_DDP this, server_HTTP_Connection conn, corto_method m, corto_object instance, JSON_Array*params) {
    corto_int32 i = 1;

    /* Allocate buffer for arguments */
    void *buffer = alloca(corto_function(m)->size);
    void *ptr = buffer;

    /* Set this pointer in buffer */
    *(corto_object*)buffer = instance;
    ptr = CORTO_OFFSET(ptr, sizeof(corto_object));

    /* Parse parameters */
    corto_parameterseqForeach(corto_function(m)->parameters, p) {
        JSON_Value *arg = json_array_get_value(params, i);

        /* Determine the JSON type, call appropriate conversion */
        switch(json_value_get_type(arg)) {
        case JSONNull: {
            corto_object n = NULL;
            corto_convert(corto_primitive(corto_object_o), &n, corto_primitive(p.type), ptr);
            break;
        }
        case JSONString: {
            corto_string str = (char*)json_value_get_string(arg);
            corto_convert(corto_primitive(corto_string_o), &str, corto_primitive(p.type), ptr);
            break;
        }
        case JSONNumber: {
            double num = json_value_get_number(arg);
            corto_convert(corto_primitive(corto_float64_o), &num, corto_primitive(p.type), ptr);
            break;
        }
        case JSONObject:
        case JSONArray: {
            server_DDP_Session_error(conn->udata, "complex arguments are not yet supported", NULL);
            goto error;
        }
        case JSONBoolean: {
            corto_bool b = json_value_get_boolean(arg);
            corto_convert(corto_primitive(corto_bool_o), &b, corto_primitive(p.type), ptr);
            break;
        }
        default:
            break;
        }

        /* Progress parameter buffer to next parameter */
        ptr = CORTO_OFFSET(ptr, corto_type_sizeof(p.type));
        i++;
    }

    /* Call method */
    corto_callb(corto_function(m), NULL, buffer);

    return;
error:
    return;
}

static corto_void server_DDP_method(server_DDP this, server_HTTP_Connection conn, JSON_Value* json) {
    corto_string reason = NULL;
    corto_object instance = NULL;
    char *method = (char*)json_object_get_string(json_object(json), "method");
    JSON_Array* params = json_object_get_array(json_object(json), "params");

    if (!params) {
        corto_asprintf(&reason, "method %s: cannot find params", method);
        goto error;
    }

    /* Is the method an instance method? */
    if (method[0] == '.') {
        /* Instance is always the first parameter in the argument list */
        char* query = (char*)json_array_get_string(params, 0);
        if (!(instance = corto_resolve(NULL, query))) {
            corto_asprintf(&reason, "Instance '%s' for method '%s' not found!", query, method);
            goto error;
        }

        /* Resolve method on instance */
        corto_method m = corto_interface_resolveMethod(corto_typeof(instance), method + 1);
        if (m) {
            /* Method found, invoke it */
            server_DDP_callMethod(this, conn, m, instance, params);
        } else {
            corto_asprintf(&reason, "Method '%s' not found in type '%s' of instance '%s'!",
                method, corto_fullpath(NULL, corto_typeof(instance)), query);
            goto error;
        }

        corto_release(instance);
    } else {
        /* Ordinary methods not yet supported */
    }

    return;
error:
    server_DDP_Session_error(conn->udata, reason, NULL);
    corto_dealloc(reason);
}
/* $end */

corto_int16 _server_DDP_construct(
    server_DDP this)
{
/* $begin(corto/web/server/DDP/construct) */
    corto_trace("[ddp] server created");

    this->sessions = corto_voidCreateChild(this, "__sessions");

    return server_SockJs_construct(this);
/* $end */
}

server_DDP_Publication _server_DDP_getPublication(
    server_DDP this,
    corto_string name)
{
/* $begin(corto/web/server/DDP/getPublication) */
    corto_id subId;
    char *ptr = subId, ch;

    /* Replace / with _ */
    strcpy(subId, name);

    while ((ch = *ptr)) {
        if (ch == '/') {
            *ptr = '_';
        }
        ptr++;
    }

    /* Find matching publication */
    server_DDP_Publication pub = corto_lookup(this, subId);
    if (!pub && this->autoPublish) {
        corto_object o = corto_resolve(NULL, name);
        if (o) {
            pub = server_DDP_PublicationCreateChild(this, subId, o);
            corto_release(o);
        }
    }

    return pub;
/* $end */
}

corto_int16 _server_DDP_init(
    server_DDP this)
{
/* $begin(corto/web/server/DDP/init) */

    /* autoPublish is by default turned on for ease of use. It should be turned
    * of in real systems because of its security implications. */
    this->autoPublish = TRUE;

    return 0;
/* $end */
}

corto_void _server_DDP_onClose(
    server_DDP this,
    server_HTTP_Connection c)
{
/* $begin(corto/web/server/DDP/onClose) */
    server_DDP_Session session = server_DDP_Session(c->udata);

    if (session) {
        corto_trace("[ddp] session %s: close session", corto_idof(session));
        corto_delete(session);
    } else {
        corto_trace("[ddp] session ???: close connection");
    }
/* $end */
}

corto_void _server_DDP_onData(
    server_DDP this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(corto/web/server/DDP/onData) */
    JSON_Value *root = json_parse_string(msg);
    if (!root) {
        goto error;
    }

    if (json_value_get_type(root) == JSONObject) {
        JSON_Object *jsonObj = json_value_get_object(root);
        const char *msg = json_object_get_string(jsonObj, "msg");
        if (!strcmp(msg, "connect")) {
            server_DDP_connect(this, c, jsonObj);
        } else if (!strcmp(msg, "ping")) {
            server_DDP_ping(this, c, jsonObj);
        } else if (!strcmp(msg, "sub")) {
            server_DDP_sub(this, c, jsonObj);
        } else if (!strcmp(msg, "unsub")) {
            server_DDP_unsub(this, c, jsonObj);
        } else if (!strcmp(msg, "method")) {
            server_DDP_method(this, c, root);
        } else {
            goto msg_error;
        }
    } else {
        goto msg_error;
    }

msg_error:
    json_value_free(root);
error:;
/* $end */
}

corto_void _server_DDP_onPoll(
    server_DDP this)
{
/* $begin(corto/web/server/DDP/onPoll) */
    corto_event e;
    corto_ll events = corto_llNew();

    /* Poll SockJs so it can send out heartbeats */
    server_SockJs_onPoll_v(this);

    /* Collect events */
    corto_lock(this);
    while ((e = corto_llTakeFirst(this->events))) {
        corto_llAppend(events, e);
    }
    corto_unlock(this);

    /* Handle events outside of lock */
    while ((e = corto_llTakeFirst(events))) {
        corto_event_handle(e);
        corto_release(e);

        /* If processing lots of events, ensure that SockJs gets a chance to
         * send out heartbeats */
        server_SockJs_onPoll_v(this);
    }

    corto_llFree(events);
/* $end */
}

/* $header(corto/web/server/DDP/post) */
static corto_observableEvent server_DDP_findRelatedEvent(server_DDP this, corto_observableEvent e) {
    corto_iter iter = corto_llIter(this->events);
    corto_observableEvent e2;
    while ((corto_iterHasNext(&iter))) {
        e2 = corto_iterNext(&iter);
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
corto_void _server_DDP_post(
    server_DDP this,
    corto_event e)
{
/* $begin(corto/web/server/DDP/post) */
    corto_uint32 size = 0;
    corto_observableEvent e2;

    /* Append new event to queue */
    corto_lock(this);

    /* Check if there is already another event in the queue for the same object.
     * if so, replace event with latest update. */
    if ((e2 = server_DDP_findRelatedEvent(this, corto_observableEvent(e)))) {
        corto_llReplace(this->events, e2, e);
        corto_release(e2);
    } else {
        corto_llAppend(this->events, e);
    }

    size = corto_llSize(this->events);
    corto_unlock(this);

    /* If queue is getting big, slow down publisher */
    if (size > SERVER_DDP_SERVER_SIZE_THRESHOLD) {
        corto_sleep(0, SERVER_DDP_SERVER_BIG_LIST_SLEEP);
    }

/* $end */
}

corto_void _server_DDP_purge(
    server_DDP this,
    corto_object observable)
{
/* $begin(corto/web/server/DDP/purge) */
    corto_lock(this);

    corto_iter iter = corto_llIter(this->events);
    while(corto_iterHasNext(&iter)) {
        corto_observableEvent e = corto_iterNext(&iter);

        /* Remove each event that belongs to a deleted collection */
        if (e->me == observable) {
            corto_llRemove(this->events, e);
        }
    }

    corto_unlock(this);
/* $end */
}
