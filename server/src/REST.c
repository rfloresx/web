/* $CORTO_GENERATED
 *
 * REST.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/web/server/server.h"

/* $header() */
#include "corto/fmt/json/json.h"

void server_REST_apiRequest(
    server_REST this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
    corto_bool value = FALSE;
    corto_bool meta = FALSE;
    corto_bool augment = FALSE;
    corto_uint64 offset = 0;
    corto_uint64 limit = 0;
    corto_string augmentFilter = NULL;
    corto_ll items = corto_llNew();
    corto_string reply = NULL;
    corto_bool multiple = FALSE;
    corto_int16 ret;

    /* Set correct content type */
    server_HTTP_Request_setHeader(
        r,
        "Content-Type",
        "application/json; charset=utf-8");

    /* Determine what to show */
    if (!strcmp(server_HTTP_Request_getVar(r, "value"), "true")) { value = TRUE; }
    if (!strcmp(server_HTTP_Request_getVar(r, "meta"), "true")) { meta = TRUE; }
    offset = atoi(server_HTTP_Request_getVar(r, "offset"));
    limit = atoi(server_HTTP_Request_getVar(r, "limit"));
    augmentFilter = server_HTTP_Request_getVar(r, "augment");
    if (*augmentFilter) {
        augmentFilter = corto_strdup(augmentFilter);
        augment = TRUE;
    } else {
        augmentFilter = NULL;
    }

    /* Select objects with URI */

    corto_string select = server_HTTP_Request_getVar(r, "select");

    multiple = (strchr(select, '*') != NULL);

    corto_iter iter = corto_select(*uri ? uri : "/", select)
      .limit(offset, limit)
      .augment(augmentFilter)
      .contentType("text/json")
      .iter(&ret);
    if (ret){
        server_HTTP_Request_setStatus(r, 400);
        server_HTTP_Request_reply(r, "400: bad request\n");
        return;
    };

    /* Add object to result list */
    corto_resultIterForeach(iter, result) {
        corto_string metaTxt = NULL;
        corto_string valueTxt = NULL;
        corto_string augmentTxt = NULL;
        corto_string item = NULL;
        if (meta) {
            if (strcmp(result.name, result.id)) {
                corto_asprintf(
                  &metaTxt,
                  "\"meta\":{\"type\":\"%s\", \"name\":\"%s\"}",
                  result.type,
                  result.name
                );
            } else {
                corto_asprintf(
                  &metaTxt,
                  "\"meta\":{\"type\":\"%s\"}",
                  result.type
                );
            }
        }

        if (augment) {
            corto_string tmp;
            corto_asprintf(
                &augmentTxt,
                ", \"augments\":[");
            if (result.augments.length) {
                corto_int32 i;
                corto_augmentData *augmentData;
                for (i = 0; i < result.augments.length; i++) {
                    augmentData = &result.augments.buffer[i];
                    corto_asprintf(
                        &tmp,
                        "%s%s{\"name\":\"%s\",\"value\":%s}",
                        augmentTxt,
                        i ? "," : "",
                        augmentData->id,
                        (corto_string)augmentData->data);
                    corto_dealloc(augmentTxt);
                    augmentTxt = tmp;
                }
            }
            corto_asprintf(
                &tmp,
                "%s]", augmentTxt);
            corto_dealloc(augmentTxt);
            augmentTxt = tmp;
        }

        if (value) {
            valueTxt = corto_result_getText(&result);
        }
        {
            corto_id id; sprintf(id, "%s/%s", result.parent, result.id);
            corto_cleanpath(id);
            corto_asprintf(
                &item,
                "{\"id\":\"%s\"%s%s%s%s%s%s}",
                id,
                metaTxt ? ", " : "",
                metaTxt ? metaTxt : "",
                valueTxt ? ", " : "",
                valueTxt ? "\"value\":" : "",
                valueTxt ? valueTxt : "",
                augmentTxt ? augmentTxt : ""
            );
            corto_dealloc(metaTxt);
            corto_llAppend(items, item);
        }
    }

    if (!corto_llSize(items)) {
        if (multiple) {
            server_HTTP_Request_reply(r, "[]");
            return;
        } else {
            corto_string msg;
            corto_asprintf(&msg, "404: resource not found '%s'", uri);
            server_HTTP_Request_setStatus(r, 404);
            server_HTTP_Request_reply(r, msg);
            corto_dealloc(msg);
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

    server_HTTP_Request_reply(r, reply);

    if (augmentFilter) {
        corto_dealloc(augmentFilter);
    }
}
/* $end */

corto_int16 _server_REST_construct(
    server_REST this)
{
/* $begin(corto/web/server/REST/construct) */
    return server_Service_construct(this);
/* $end */
}

corto_int16 _server_REST_onRequest(
    server_REST this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/REST/onRequest) */
    server_REST_apiRequest(this, c, r, uri);
    return 1;
/* $end */
}
