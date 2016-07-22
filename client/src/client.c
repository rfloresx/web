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
/* $end */

web_client_Result _web_client_get(
    corto_string url,
    corto_string fields)
{
/* $begin(corto/web/client/get) */
    web_client_Result result = {0, NULL};
    return result;
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
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        if (fields) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(fields));
        }

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            corto_seterr("curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
        }

        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &result.status);

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
