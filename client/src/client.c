/* $CORTO_GENERATED
 *
 * client.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/client/client.h>

/* $header() */
#include <curl/curl.h>

#define INITIAL_BODY_BUFFER_SIZE (512)

struct url_data {
    size_t size;
    char* buffer;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;
    data->size += (size * nmemb);
    tmp = corto_realloc(data->buffer, data->size + 1);
    if (tmp) {
        data->buffer = tmp;
    } else {
        if (data->buffer) {
            corto_dealloc(data->buffer);
        }
        goto error;
    }

    memcpy((data->buffer + index), ptr, n);
    data->buffer[data->size] = '\0';

    return size * nmemb;
error:
    return 0;
}

/* $end */

web_client_Result _web_client_get(
    corto_string url,
    corto_string fields)
{
/* $begin(corto/web/client/get) */
    web_client_Result result = {0, NULL};
    CURL *curl = curl_easy_init();
    if (!curl) {
        corto_seterr("could not init curl");
        goto error;
    }
    struct url_data data = {0, NULL};
    data.buffer = corto_alloc(INITIAL_BODY_BUFFER_SIZE);
    if (!data.buffer) {
        goto error;
    }
    data.buffer[0] = '\0';
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        corto_seterr("curl_easy_perform() failed: %s", curl_easy_strerror(res));
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result.status);
    result.response = data.buffer;
    curl_easy_cleanup(curl);

    return result;
error:
    return (web_client_Result){0, NULL};
/* $end */
}

web_client_Result _web_client_post(
    corto_string url,
    corto_string fields)
{
/* $begin(corto/web/client/post) */
    CURL *curl;
    CURLcode res;
    web_client_Result result;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        if (fields) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(fields));
        }

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            corto_seterr("curl_easy_perform() failed: %s", curl_easy_strerror(res));
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result.status);

        curl_easy_cleanup(curl);
    }

    /* Not supported yet */
    result.response = NULL;

    return result;
/* $end */
}

int clientMain(int argc, char* argv[]) {
/* $begin(main) */

    curl_global_init(CURL_GLOBAL_DEFAULT);

    return 0;
/* $end */
}
