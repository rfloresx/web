/* $CORTO_GENERATED
 *
 * web.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/web/web.h>

/* $header() */
typedef struct web_escape {
    char ch;
    char *escapeStr;   
} web_escape;

static web_escape web_escapes[] = {
    {' ', "20"},
    {'$', "24"},
    {'&', "26"},
    {'`', "60"},
    {':', "3A"},
    {'<', "3C"},
    {'>', "3E"},
    {'[', "5B"},
    {']', "5D"},
    {'{', "7B"},
    {'}', "7D"},
    {'"', "22"},
    {'+', "2B"},
    {'#', "23"},
    {'%', "25"},
    {'@', "40"},
    {'/', "2F"},
    {';', "3B"},
    {'=', "3D"},
    {'?', "3F"},
    {'\\', "5C"},
    {'^', "5E"},
    {'|', "7C"},
    {'~', "7E"},
    {'\'', "27"},
    {',', "2C"},
    {'\0', NULL}
};

char web_findEscapeCharacter(char *escapeStr) {
    corto_int32 i;
    for (i = 0; (web_escapes[i].ch); i++) {
        if (!memcmp(escapeStr, web_escapes[i].escapeStr, 2)) {
            return web_escapes[i].ch;
        }
    }
    return '\0';
}

char* web_findEscapeString(char ch) {
    corto_int32 i;
    for (i = 0; (web_escapes[i].ch); i++) {
        if (ch == web_escapes[i].ch) {
            return web_escapes[i].escapeStr;
        }
    }
    return NULL;
}

/* $end */

corto_string _web_escapeFromRequest(
    corto_string data)
{
/* $begin(corto/web/escapeFromRequest) */
    char *ptr = data, *out = NULL, *outPtr = NULL, ch, escapeCh;

    while ((ch = *ptr)) {
        switch(ch) {
        case '%':
            if (!(escapeCh = web_findEscapeCharacter(ptr + 1))) {
                corto_seterr("unrecognized escape sequence '%c%c'", ptr[1], ptr[2]);
                goto error;
            }
            if (!out) {
                out = corto_strdup(data);
                outPtr = &out[ptr - data];
            }
            *outPtr = escapeCh;
            ptr += 2;
            break;
        default:
            if (outPtr) *outPtr = ch;
            break;
        }

        if (outPtr) {
            outPtr ++;
        }
        ptr ++;
    }

    if (outPtr) {
        *outPtr = '\0';
    }

    return out;
error:
    return NULL;
/* $end */
}

corto_string _web_escapeToRequest(
    corto_string url)
{
/* $begin(corto/web/escapeToRequest) */
    return NULL;
/* $end */
}

int webMain(int argc, char *argv[]) {
/* $begin(main) */

    /* Insert code that must be run when component is loaded */

    return 0;
/* $end */
}
