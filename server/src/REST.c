/* $CORTO_GENERATED
 *
 * REST.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/server/server.h>

/* $header() */
#include "corto/fmt/json/json.h"

void server_REST_apiGet(
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
    corto_string typeFilter = NULL;
    corto_bool multiple = FALSE;
    corto_int16 ret;

    corto_buffer response = CORTO_BUFFER_INIT;

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
    typeFilter = server_HTTP_Request_getVar(r, "type");
    if (*augmentFilter) {
        augmentFilter = corto_strdup(augmentFilter);
        augment = TRUE;
    } else {
        augmentFilter = NULL;
    }
    if (*typeFilter) {
        typeFilter = corto_strdup(typeFilter);
    } else {
        typeFilter = NULL;
    }

    /* Select objects with URI */
    corto_string select = server_HTTP_Request_getVar(r, "select");
    multiple = (strchr(select, '*') != NULL);

    corto_trace("REST: select(%s, %s).limit(%d, %d).type(%s).contentType(%s)",
      *uri ? uri : "/", select, offset, limit, typeFilter ? typeFilter : "*", "text/json");

    corto_iter iter;
    ret = corto_select(*uri ? uri : "/", select)
      .limit(offset, limit)
      .augment(augmentFilter)
      .type(typeFilter)
      .contentType("text/json")
      .iter(&iter);
    if (ret) {
        server_HTTP_Request_setStatus(r, 400);
        server_HTTP_Request_reply(r, "400: bad request\n");
        return;
    };


    /* Add object to result list */
    corto_uint32 count = 0;
    if (multiple) {
        corto_buffer_append(&response, "[");
    }

    corto_resultIterForeach(iter, result) {
        /* Skip hidden objects */
        if (*result.id == '.') continue;

        if (count) {
          corto_buffer_append(&response, ",");
        }

        corto_buffer_append(&response, "{\"id\":\"%s\"", result.id);
        if (meta) {
            corto_buffer_append(&response, ",\"meta\":{\"type\":\"%s\"",
              result.type);
            if (strcmp(result.name, result.id)) {
                corto_buffer_append(&response , ",\"name\":\"%s\"",
                  result.name);
            }
            if (strcmp(result.parent, ".")) {
                corto_buffer_append(&response , ",\"parent\":\"%s\"",
                  result.parent);
            }
            if (result.mount) {
                corto_buffer_append(&response , ",\"owner\":\"%s\"",
                  corto_fullpath(NULL, result.mount));
            }
            corto_buffer_append(&response, "}");
        }

        if (augment) {
            corto_buffer_append( &response, ",\"augments\":[");
            if (result.augments.length) {
                corto_int32 i;
                corto_augmentData *augmentData;
                for (i = 0; i < result.augments.length; i++) {
                    augmentData = &result.augments.buffer[i];
                    corto_buffer_append(
                        &response,
                        "%s%s{\"name\":\"%s\",\"value\":%s}",
                        i ? "," : "",
                        augmentData->id,
                        (corto_string)augmentData->data);
                }
            }
            corto_buffer_append(&response, "]");
        }

        if (value) {
            corto_string valueTxt = corto_result_getText(&result);
            if (valueTxt) {
                corto_buffer_append(&response, ",\"value\":%s", valueTxt);
            }
        }
        corto_buffer_append(&response, "}");
        count ++;
    }

    if (multiple) {
        corto_buffer_append(&response, "]");
    }

    if (!count && !multiple) {
        corto_string msg;
        corto_asprintf(&msg, "404: resource not found '%s'", uri);
        server_HTTP_Request_setStatus(r, 404);
        server_HTTP_Request_reply(r, msg);
        corto_dealloc(msg);
        return;
    }

    corto_string responseStr = corto_buffer_str(&response);
    server_HTTP_Request_reply(r, responseStr);

    if (augmentFilter) {
        corto_dealloc(augmentFilter);
    }

    corto_dealloc(responseStr);
}
/* $end */

corto_int16 _server_REST_construct(
    server_REST this)
{
/* $begin(corto/web/server/REST/construct) */
    return server_Service_construct(this);
/* $end */
}

corto_int16 _server_REST_onGet(
    server_REST this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/REST/onGet) */
    server_REST_apiGet(this, c, r, uri);
    return 1;
/* $end */
}

corto_int16 _server_REST_onPost(
    server_REST this,
    server_HTTP_Connection c,
    server_HTTP_Request *r,
    corto_string uri)
{
/* $begin(corto/web/server/REST/onPost) */

    return 0;

/* $end */
}
