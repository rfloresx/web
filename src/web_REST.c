/* $CORTO_GENERATED
 *
 * web_REST.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "json.h"

void web_REST_apiRequest(
    web_REST this,
    web_HTTP_Connection c,
    web_HTTP_Request *r)
{
    corto_bool value = FALSE;
    corto_bool meta = FALSE;
    corto_ll items = corto_llNew();
    corto_string reply = NULL;
    corto_bool multiple =
      (strchr(r->uri, '*') != NULL);

    /* Set correct content type */
    web_HTTP_Request_setHeader(
        r,
        "Content-Type",
        "application/json; charset=utf-8");

    /* Determine what to show */
    if (!strcmp(web_HTTP_Request_getVar(r, "value"), "true")) { value = TRUE; }
    if (!strcmp(web_HTTP_Request_getVar(r, "meta"), "true")) { meta = TRUE; }

    /* Select objects with URI */
    corto_iter iter;
    if (corto_select(root_o, r->uri + 4, &iter)) {
        web_HTTP_Request_setStatus(r, 400);
        web_HTTP_Request_reply(r, "400: bad request\n");
        return;
    }

    if (corto_selectContentType(&iter, "text/json")) {
        corto_string msg;
        web_HTTP_Request_setStatus(r, 500);
        corto_asprintf(&msg,
            "500: failed to set contentType to text/json\n"
            "    details: %s\n", corto_lasterr());
        web_HTTP_Request_reply(r, msg);
        corto_dealloc(msg);
        return;
    }

    /* Add object to result list */
    corto_resultIterForeach(iter, result) {
        corto_string metaTxt = NULL;
        corto_string valueTxt = NULL;
        corto_string item = NULL;
        if (meta) {
            corto_asprintf(
              &metaTxt,
              "\"meta\":{\"type\":\"%s\"}",
              result.name,
              result.parent,
              result.type
            );
        }
        if (value) {
            valueTxt = corto_result_getText(&result);
        }

        {
            corto_id id; sprintf(id, "%s/%s", result.parent, result.name);
            corto_cleanpath(id);
            corto_asprintf(
                &item,
                "{\"_id\":\"%s\"%s%s%s%s%s}",
                id,
                metaTxt ? ", " : "",
                metaTxt ? metaTxt : "",
                valueTxt ? ", " : "",
                valueTxt ? "\"value\":" : "",
                valueTxt ? valueTxt : ""
            );
            corto_dealloc(metaTxt);
            corto_llAppend(items, item);
        }
    }

    if (!corto_llSize(items)) {
        if (multiple) {
            web_HTTP_Request_reply(r, "{}");
            return;
        } else {
            web_HTTP_Request_setStatus(r, 404);
            web_HTTP_Request_reply(r, "404: resource not found");
            return;
        }
    }

    if (multiple) {
        reply = corto_strdup("[");
    }

    /* Merge items into one result string */
    corto_int32 count = 0;
    iter = corto_llIter(items);
    while (corto_iterHasNext(&iter)) {
        corto_string item = corto_iterNext(&iter);
        corto_string temp = NULL;

        /* Wildly inefficient */
        if (reply) {
            if (!count) {
                corto_asprintf(
                    &temp,
                    "%s%s",
                    reply,
                    item);
            } else {
                corto_asprintf(
                    &temp,
                    "%s, %s",
                    reply,
                    item);
            }
            corto_dealloc(reply);
            reply = temp;
        } else {
            reply = corto_strdup(item);
        }
        count++;
    }


    if (multiple) {
        corto_string temp = NULL;
        corto_asprintf(
            &temp,
            "%s]",
            reply);
        corto_dealloc(reply);
        reply = temp;
    }

    web_HTTP_Request_reply(r, reply);

    /* Serialize object-value to JSON */
    /*struct corto_serializer_s serializer = corto_json_ser(
        CORTO_PRIVATE,
        CORTO_NOT,
        CORTO_SERIALIZER_TRACE_NEVER);

    if ((corto_typeof(o)->kind == CORTO_VOID) && (!meta && !expr)) {
        web_HTTP_Request_reply(r, "\n");
    } else {
        corto_json_ser_t jsonData = {
          NULL, NULL, 0, 0, 0, meta, value, scope, TRUE};
        corto_serialize(&serializer, o, &jsonData);
        web_HTTP_Request_reply(r, jsonData.buffer);
        corto_dealloc(jsonData.buffer);
    }

    corto_release(o);*/
}
/* $end */

corto_int16 _web_REST_construct(web_REST this) {
/* $begin(corto/web/REST/construct) */

    return web_Service_construct(this);

/* $end */
}

corto_void _web_REST_onRequest(web_REST this, web_HTTP_Connection c, web_HTTP_Request *r) {
/* $begin(corto/web/REST/onRequest) */
    CORTO_UNUSED(this);

    if (!memcmp(r->uri, "/api", 4)) {
        web_REST_apiRequest(this, c, r);
    } else {
        web_HTTP_Request_reply(r, "invalid URI");
    }

/* $end */
}
