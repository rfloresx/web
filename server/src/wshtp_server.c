/******************************************************************************
 *
 * Copyright (c) 2016 Roberto Flores.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "wshtp_server.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <event2/thread.h>

#define FIN      0x80

#define OP_CONT  0x00
#define OP_TEXT  0x01
#define OP_BIN   0X02
//OP_RESERVED 0x3-7
#define OP_CLOSE 0x08
#define OP_PING  0x09
#define OP_PONG  0x0A
//OP_RESERVED 0xB-F
#define MASK 0x80


#define SAFE_FREE(ptr) if (ptr != NULL) {free(ptr);}

/* ***********************************
 * Private Structs:
 * ***********************************/
// frame link list
typedef struct frame_node_s {
    void *data;
    size_t length;
    struct frame_node_s *next;
} frame_node_t;

// root frame buffer
struct ws_buffer_s {
    frame_node_t *root;
    frame_node_t **head;
};

// create a new frame buffer object
ws_buffer_t *ws_buffer_new();

// clean managed data without releasing the frame buffer object
void ws_buffer_reset(ws_buffer_t *fbuf);

// release frame buffer object and clean memory
void ws_buffer_free(ws_buffer_t *fbuf);

// insert data to the frame buffer object
// the frame buffer take ownership of the data don't delete passed data
void ws_buffer_append_data(ws_buffer_t *fbuf, void *data, size_t length);

// get the data of the ws_buffer in a continuous array
void* ws_buffer_get_data(ws_buffer_t *fbuf, size_t *length);


//
typedef struct wshtp_hook_s {
    bool valid;
    wshtp_hook_cb cb;
    void *data;
} wshtp_hook_t;

struct wshtp_hooks_s {
    wshtp_hook_t on_open;
    wshtp_hook_t on_close;
    wshtp_hook_t on_get;
    wshtp_hook_t on_post;
    wshtp_hook_t on_put;
    wshtp_hook_t on_delete;
    wshtp_hook_t on_message;
    wshtp_hook_t on_connection;
};

enum frame_status_e {
    STATUS_ERROR = -1,
    UNKNOWN = 0,
    STATUS_OK = 1
};

typedef struct ws_frame_s {
    enum frame_status_e status;
    bool fin;
    bool RSV1;
    bool RSV2;
    bool RSV3;
    int8_t opcode;
    bool mask;
    int32_t mask_key;

    int64_t payload_len;
    char *data;
} ws_frame_t;

/* ***********************************
 * Private Methods
 * ***********************************/

static wshtp_conn_t *wshtp_conn_new(wshtp_server_t *server, evhtp_connection_t *conn);
static void wshtp_conn_free(wshtp_conn_t *conn);
static void wshtp_conn_set_request(wshtp_conn_t *conn, evhtp_request_t *req);


//Websocket
static int do_ws_handshake(evhtp_request_t *req);
static void ws_read_cb(evbev_t * bev, void *arg);
static void ws_event_cb(evbev_t *bev, short events, void *args);
static size_t ws_parse_frame(const char *data, size_t data_len, ws_frame_t *frame);

static int ws_send_frame(wshtp_conn_t *conn, bool fin, uint8_t opcode,
                         const void *data, size_t size,
                         bool maks, uint32_t mask_key);

//Callbacks
static void wshtp_hook_set(wshtp_hook_t *hook, wshtp_hook_cb cb, void *data);
static int wshtp_hooks_call(wshtp_server_t *server, enum ws_hook_type_e type, wshtp_conn_t **conn);
static int wshtp_hook_call(wshtp_hook_t *hook, wshtp_conn_t *conn);

//
static void ws_headers_add_header(evhtp_headers_t *header, const char *key, const char *val);

static const char *wshtp_get_method_str(htp_method method);

//Utility
static int strncmpcase(const char *str1, const char *str2, size_t num);
static char *strstrcase(const char *str1, const char *str2);
static int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize);

static const char *mime_types[][2] = {
    {".html",	"text/html"},
    {".htm",	"text/html"},
    {".shtm",	"text/html"},
    {".shtml",	"text/html"},
    {".css",	"text/css"},
    {".txt",	"text/plain"},
    {".xml",	"text/xml"},

    {".ico",	"image/x-icon"},
    {".gif",	"image/gif"},
    {".jpg",	"image/jpeg"},
    {".jpeg",	"image/jpeg"},
    {".png",	"image/png"},
    {".svg",	"image/svg+xml"},
    {".bmp",	"image/bmp"},

    {".wav",	"audio/x-wav"},
    {".mp3",	"audio/x-mp3"},
    {".mid",	"audio/mid"},
    {".m3u",	"audio/x-mpegurl"},
    {".ra",	    "audio/x-pn-realaudio"},
    {".ram",	"audio/x-pn-realaudio"},

    {".js",     "application/javascript"},
    {".ogg",	"application/ogg"},
    {".json",	"application/json"},
    {".xslt",	"application/xml"},
    {".xsl",	"application/xml"},
    {".doc",	"application/msword"},
    {".exe",	"application/octet-stream"},
    {".zip",	"application/x-zip-compressed"},
    {".xls",	"application/excel"},
    {".tgz",	"application/x-tar-gz"},
    {".tar",	"application/x-tar"},
    {".gz",	    "application/x-gunzip"},
    {".arj",	"application/x-arj-compressed"},
    {".rar",	"application/x-rar-compressed"},
    {".rtf",	"application/rtf"},
    {".pdf",	"application/pdf"},
    {".swf",	"application/x-shockwave-flash"},
    {".ttf",	"application/x-font-ttf"},

    {".mpg",	"video/mpeg"},
    {".webm",	"video/webm"},
    {".mpeg",	"video/mpeg"},
    {".mov",	"video/quicktime"},
    {".mp4",	"video/mp4"},
    {".m4v",	"video/x-m4v"},
    {".asf",	"video/x-ms-asf"},
    {".avi",	"video/x-msvideo"},
    {NULL, NULL}
};

static const char *get_mime_type(const char *file_name) {
    const char * ret = NULL;
    const char *ptr = strrchr(file_name, '.');

    if (ptr != NULL) {
        int len = strlen(ptr)+1;
        const char **curr = mime_types[0];
        while (curr[0] != NULL){
            if(strncmpcase(ptr, curr[0], len) == 0) {
                ret = curr[1];
                break;
            }
            curr = curr + 1;
        }
    }
    return ret;
}

/* **********************************
 * Server
 * **********************************/
static evhtp_res wshtp_post_accept_cb(evhtp_connection_t *conn, void *arg) {
    wshtp_server_t *server = (wshtp_server_t*)arg;
    int64_t id = (intptr_t)(conn);

    wshtp_conn_t *wsconn = (wshtp_conn_t*)avlmap_find(server->conns, id);
    if (wsconn == NULL){
        wsconn =  wshtp_conn_new(server, conn);
        avlmap_insert(server->conns, id, wsconn);
    }
    return EVHTP_RES_OK;
}

static void wshtp_handler_cb(evhtp_request_t *req, void *data) {
    evhtp_connection_t *conn = evhtp_request_get_connection(req);
    wshtp_server_t *server = (wshtp_server_t*)data;

    wshtp_conn_t *_conn = (wshtp_conn_t*)avlmap_find(server->conns, (intptr_t)conn);

    wshtp_conn_set_request(_conn, req);
    if (req->method == htp_method_GET) {
        int ret = do_ws_handshake(req);
        if (ret == EVHTP_RES_SWITCH_PROTO) {
            _conn->is_websocket = true;

            evbev_t * bev = evhtp_connection_take_ownership(conn);
            evhtp_connection_set_bev(conn, bev);
            conn->owner = 1;

            bufferevent_enable(bev, EV_READ);
            bufferevent_setcb(bev, ws_read_cb, NULL, ws_event_cb, _conn);
        } else if (ret != EVHTP_RES_OK) {
            return;
        }
        _conn->method = WSHTP_ON_GET;
    } else if (req->method == htp_method_POST) {
        _conn->method = WSHTP_ON_POST;
    } else if (req->method == htp_method_PUT) {
        _conn->method = WSHTP_ON_PUT;
    } else if (req->method == htp_method_DELETE) {
        _conn->method = WSHTP_ON_DELETE;
    }

    if (_conn->is_websocket == false && _conn->method) {
        wshtp_hooks_call(server, _conn->method, &_conn);
    } else {
        wshtp_hooks_call(server, WSHTP_ON_OPEN, &_conn);
    }
}

static wshtp_conn_t *wshtp_conn_new(wshtp_server_t *server, evhtp_connection_t *conn) {
    wshtp_conn_t *wsconn = calloc(1,sizeof(wshtp_conn_t));
    if (conn == NULL) {
        return NULL;
    }

    wsconn->server = server;
    wsconn->conn = conn;

    return wsconn;
}

static void wshtp_conn_free(wshtp_conn_t *conn) {
    if (conn) {
        if (conn->free_cb && conn->userdata) {
            conn->free_cb(conn, conn->userdata);
        }
        if (conn->data.ws_frames) {
            ws_buffer_free(conn->data.ws_frames);
            conn->data.ws_frames = NULL;
        }
        if (conn->data.content) {
            free(conn->data.content);
        }
        free(conn);
    }
}

static void wshtp_conn_set_request(wshtp_conn_t *conn, evhtp_request_t *req) {
    conn->request = req;
    conn->reply.code = 0;
    conn->reply.headers = req->headers_out;
    conn->reply.out = req->buffer_out;
}

/* **********************************
 * Websocket
 * **********************************/
static int do_ws_handshake(evhtp_request_t *req) {
    const char *upgrade = evhtp_header_find(req->headers_in, "Upgrade");
    const char *connection = evhtp_header_find(req->headers_in, "Connection");
    const char *ws_key = evhtp_header_find(req->headers_in, "Sec-WebSocket-Key");

    if (upgrade && connection && ws_key) {
        if (!strstrcase(upgrade, "websocket") ||
            !strstrcase(connection, "Upgrade")){
            return EVHTP_RES_ERROR;
        }
        static const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        unsigned char *str = calloc(strlen(ws_key)+strlen(magic)+1, sizeof(unsigned char));

        strcpy((char *)str, ws_key);
        strcat((char *)str, magic);

        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(str, strlen((char *)str), hash);

        free(str);

        char ws_accept[29];
        ws_accept[28] = '\0';
        base64encode(&hash[0], sizeof(hash), &ws_accept[0], sizeof(ws_accept)-1);

        ws_headers_add_header(req->headers_out, "Connection", "Upgrade" );
        ws_headers_add_header(req->headers_out, "Sec-WebSocket-Accept", &ws_accept[0]);
        ws_headers_add_header(req->headers_out, "Upgrade", "websocket");
        evhtp_send_reply(req, EVHTP_RES_SWITCH_PROTO);
        return EVHTP_RES_SWITCH_PROTO;
    }
    return EVHTP_RES_OK;
}

static void ws_read_cb(evbev_t * bev, void *arg) {
    size_t data_len = 0;

    wshtp_conn_t *conn = (wshtp_conn_t*)arg;

    evbuf_t *in = bufferevent_get_input(bev);

    bufferevent_lock(bev);

    while ( (data_len = evbuffer_get_length(in)) ){
        void *data = evbuffer_pullup(in, data_len);

        ws_frame_t frame;

        size_t nread = ws_parse_frame(data, data_len, &frame);

        if (frame.status == STATUS_OK) {

            if (frame.opcode == OP_TEXT) {
                conn->data.type = WS_DATA_TEXT;
                conn->method = WSHTP_ON_MESSAGE;
            } else if (frame.opcode == OP_BIN) {
                conn->data.type = WS_DATA_BINARY;
                conn->method = WSHTP_ON_MESSAGE;
            } else if (frame.opcode == OP_CLOSE) {
                conn->method = WSHTP_ON_CLOSE;
            }

            if (frame.fin == 0 || frame.opcode == OP_CONT) {
                if (conn->data.ws_frames == NULL) {
                    conn->data.ws_frames = ws_buffer_new();
                }
                ws_buffer_append_data(conn->data.ws_frames, frame.data, frame.payload_len);
            }

            if (conn->method == WSHTP_ON_CLOSE) {
                evbuffer_drain(in, data_len);
            } else {
                evbuffer_drain(in, nread);
            }

            if (frame.fin) {
                if (conn->data.ws_frames) {
                    conn->data.content = ws_buffer_get_data(conn->data.ws_frames, &conn->data.size);
                    ws_buffer_free(conn->data.ws_frames);
                    conn->data.ws_frames = NULL;
                } else {
                    conn->data.content = frame.data;
                    conn->data.size = frame.payload_len;
                }

                bufferevent_unlock(bev);

                wshtp_hooks_call(conn->server, conn->method, &conn);

                bufferevent_lock(bev);

                if (conn) {
                    free(conn->data.content);
                    conn->data.content = NULL;
                    conn->data.size = 0;
                }
            }
        } else {
            break;
        }
    }

    // free fragments
    bufferevent_unlock(bev);
}

static void ws_event_cb(evbev_t *bev, short what, void *args) {
    wshtp_conn_t *conn = args;

    bufferevent_lock(bev);
    if (what & BEV_EVENT_EOF || what & BEV_EVENT_ERROR || what & BEV_EVENT_TIMEOUT) {
        conn->method = WSHTP_ON_CLOSE;

        bufferevent_unlock(bev);

        wshtp_hooks_call(conn->server, conn->method, &conn);

        bufferevent_lock(bev);
    }
    bufferevent_unlock(bev);
}


static size_t ws_parse_frame(const char *data, size_t data_len, ws_frame_t *frame) {
    if (data_len < 2) {
        frame->status = STATUS_ERROR;
        return 0;
    }
    frame->status = STATUS_OK;
    frame->fin  = data[0] & FIN;
    frame->RSV1 = data[0] & 0x40;
    frame->RSV2 = data[0] & 0x20;
    frame->RSV3 = data[0] & 0x10;
    frame->opcode = data[0] & 0x0F;

    frame->mask = data[1] & MASK;
    frame->payload_len = data[1] & 0x7F;

    frame->data = NULL;

    int index = 2;
    if (frame->payload_len == 127) {
        if (data_len < 10) {
            frame->status = STATUS_ERROR;
            return 0;
        }
        frame->payload_len  = ((uint64_t)(uint8_t)data[index++]) << 56;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]) << 48;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]) << 40;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]) << 32;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]) << 24;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]) << 16;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]) << 8;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]);
    }else if (frame->payload_len == 126) {
        if (data_len < 4) {
            frame->status = STATUS_ERROR;
            return 0;
        }
        frame->payload_len  = ((uint64_t)(uint8_t)data[index++]) << 8;
        frame->payload_len += ((uint64_t)(uint8_t)data[index++]);
    }

    if (frame->mask) {
        if (data_len < index + 4) {
            frame->status = STATUS_ERROR;
            return 0;
        }
        frame->mask_key = *((int32_t*)&data[index]);
        index += 4;
    }

    if (frame->payload_len > 0 ) {
        if ((index + frame->payload_len) > data_len) {
            frame->status = STATUS_ERROR;
            return 0;
        }

        frame->data = malloc(frame->payload_len + 1);
        memcpy(frame->data, &data[index], frame->payload_len+1);

        if (frame->mask) {
            char *mask_key = (char *)&frame->mask_key;
            for (int i = 0; i < frame->payload_len; i++) {
                int j = i % 4;
                frame->data[i] = frame->data[i] ^ mask_key[j];
            }
        }
        frame->data[frame->payload_len] = '\0';
        index += frame->payload_len;
    }
    return index;
}

static int ws_send_frame(wshtp_conn_t *conn, bool fin, uint8_t opcode,
                         const void *data, size_t size,
                         bool mask, uint32_t mask_key) {
    uint8_t byte0 = opcode;
    if (fin) {
        byte0 |= FIN;
    }
    uint8_t byte1 = 0; // maks_flag(1 bit) + payload_len(7 bits)

    size_t frame_size = 2;

    union {
        uint16_t len_16;
        uint64_t len_64;
        char bytes[8];
    } ext_payload_len;

    if (size > INT16_MAX) {
        byte1 = 127;
        frame_size += 8;
        ext_payload_len.len_64 = (uint64_t)size;
    }else if (size > 125) {
        byte1 = 126;
        frame_size += 2;
        ext_payload_len.len_16 = (uint16_t)size;
    }else {
        byte1 = (uint8_t)size;
    }
    if (mask) {
        frame_size += 4;
        byte1 |= MASK; //set mask
    }
    frame_size += size;
    char *frame_data = (char *)calloc(frame_size, sizeof(char)); //allocate frame data

    int index = 0;
    frame_data[index++] = byte0;
    frame_data[index++] = byte1;

    //SET extended payload_len if required
    if (size > INT16_MAX) {
        frame_data[index++] = ext_payload_len.bytes[7];
        frame_data[index++] = ext_payload_len.bytes[6];
        frame_data[index++] = ext_payload_len.bytes[5];
        frame_data[index++] = ext_payload_len.bytes[4];
        frame_data[index++] = ext_payload_len.bytes[3];
        frame_data[index++] = ext_payload_len.bytes[2];
        frame_data[index++] = ext_payload_len.bytes[1];
        frame_data[index++] = ext_payload_len.bytes[0];
    } else if (size > 125) {
        frame_data[index++] = ext_payload_len.bytes[1];
        frame_data[index++] = ext_payload_len.bytes[0];
    }

    //SET mask_key if MASK is set
    if (mask) {
        int32_t *i32 = (int32_t*)&frame_data[index];
        *i32 = mask_key;
        index += 4;
    }
    memcpy(&frame_data[index], data, size);

    evbev_t *bev =evhtp_connection_get_bev(conn->conn);
    evbuf_t *reply_buf = conn->conn->scratch_buf;

    evbuffer_drain(reply_buf, -1);
    evbuffer_add(reply_buf, &frame_data[0], frame_size);
    bufferevent_lock(bev);
    {
        bufferevent_write_buffer(bev, reply_buf);
    }
    bufferevent_unlock(bev);
    evbuffer_drain(reply_buf, -1);
    free(frame_data);
    return 0;
}

/* *********************************************
 * Callbacks
 * *********************************************/

static void wshtp_hook_set(wshtp_hook_t *hook, wshtp_hook_cb cb, void *data) {
    hook->cb = cb;
    hook->data = data;
}

static int wshtp_hooks_call(wshtp_server_t *server, enum ws_hook_type_e type, wshtp_conn_t **conn_ptr) {
    if (server == NULL) {
        return 0;
    }
    wshtp_conn_t *conn = *conn_ptr;
    wshtp_hooks_t *hooks = server->hooks;
    int ret = 0;
    switch (type) {
        case WSHTP_ON_OPEN:
            ret = wshtp_hook_call(&hooks->on_open, conn);
            break;
        case WSHTP_ON_CLOSE:
            ret = wshtp_hook_call(&hooks->on_close, conn);
            break;
        case WSHTP_ON_GET:
            ret = wshtp_hook_call(&hooks->on_get, conn);
            break;
        case WSHTP_ON_POST:
            ret = wshtp_hook_call(&hooks->on_post, conn);
            break;
        case WSHTP_ON_PUT:
            ret = wshtp_hook_call(&hooks->on_put, conn);
            break;
        case WSHTP_ON_DELETE:
            ret = wshtp_hook_call(&hooks->on_delete, conn);
            break;
        case WSHTP_ON_MESSAGE:
            ret = wshtp_hook_call(&hooks->on_message, conn);
            break;
        case WSHTP_ON_CONNECTION:
            ret = wshtp_hook_call(&hooks->on_connection, conn);
            break;
    }

    if (ret != EVHTP_RES_OK || type == WSHTP_ON_CLOSE) {
        avlmap_remove(server->conns, (intptr_t)(conn->conn));
        if (conn->is_websocket) {
            evhtp_connection_free(conn->conn);
        }
        wshtp_conn_free(conn);
        *conn_ptr = NULL;
    }
    return ret;
}

static int wshtp_hook_call(wshtp_hook_t *hook, wshtp_conn_t *conn) {
     int ret = EVHTP_RES_OK;
     if (hook->cb) {
         ret = hook->cb(conn, hook->data);
     }
     return ret;
 }

static void ws_headers_add_header(evhtp_headers_t *headers, const char *key, const char *val) {
    evhtp_headers_add_header(headers, evhtp_header_new(key, val, 1, 1));
}

static const char *wshtp_get_method_str(htp_method method) {
    char *str = NULL;
    switch (method) {
        case htp_method_GET:
        {
            str = "GET";
            break;
        }
        case htp_method_POST:
        {
            str = "POST";
            break;
        }
        case htp_method_PUT:
        {
            str = "POST";
            break;
        }
        case htp_method_DELETE:
        {
            str = "DELETE";
            break;
        }
        default:
        {
            str = NULL;
            break;
        }
    }
    return str;
}

/* **********************************************
 * Utility
 * **********************************************/
static int strncmpcase(const char *str1, const char *str2, size_t num) {
    int ret = 0;
    size_t count = 0;

    while (!ret && count < num && str1 && str2) {
        char a = toupper(*str1);
        char b = toupper(*str2);
        ret = a - b;
        if (*str1) {
            str1++;
        } else {
            str1 = NULL;
        }
        if (*str2) {
            str2++;
        }else{
            str2 = NULL;
        }
        count++;
    }
    return ret;
}

static char *strstrcase(const char *str1, const char *str2) {
    if (str1 && str2) {
        char *ptr1 = (char*)str1;
        char c2 = toupper(*str2);
        while (*ptr1) {
            char c1 = toupper(*ptr1);
            if (c1 == c2) {
                bool found = true;
                char *ptr2 = (char*)str2;
                char *ptr3 = ptr1;
                ptr2++; ptr3++;
                while(*ptr2 && found) {
                    if (!(*ptr3)) {
                        return NULL;
                    }
                    char c = toupper(*ptr2);
                    char d = toupper(*ptr3);
                    if (c != d) {
                        found = false;
                    }
                    ptr2++; ptr3++;
                }
                if (found) {
                    return ptr1;
                }
            }
            ptr1++;
        }
    }
    return NULL;
}

static int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize)
{
   //https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C
   const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   size_t x;
   uint32_t n = 0;
   int padCount = dataLength % 3;
   uint8_t n0, n1, n2, n3;

   /* increment over the length of the string, three characters at a time */
   for (x = 0; x < dataLength; x += 3)
   {
      /* these three 8-bit (ASCII) characters become one 24-bit number */
      n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

      if((x+1) < dataLength)
         n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

      if((x+2) < dataLength)
         n += data[x+2];

      /* this 24-bit number gets separated into four 6-bit numbers */
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;

      /*
       * if we have one byte available, then its encoding is spread
       * out over two characters
       */
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n0];
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n1];

      /*
       * if we have only two bytes available, then their encoding is
       * spread out over three chars
       */
      if((x+1) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n2];
      }

      /*
       * if we have all three bytes available, then their encoding is spread
       * out over four characters
       */
      if((x+2) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n3];
      }
   }

   /*
    * create and add padding that is required if we did not have a multiple of 3
    * number of characters available
    */
   if (padCount > 0)
   {
      for (; padCount < 3; padCount++)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = '=';
      }
   }
   if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
   result[resultIndex] = 0;
   return 0;   /* indicate success */
}



/* *****************************************************
 * Public API
 * *****************************************************/

wshtp_server_t *wshtp_server_new() {
    wshtp_server_t *ret = calloc(1, sizeof(wshtp_server_t));

    if (ret == NULL) {
        return NULL;
    }
    evthread_use_pthreads();

    ret->hooks = calloc(1, sizeof(wshtp_hooks_t));
    ret->evbase = event_base_new();
    ret->htp = evhtp_new(ret->evbase, NULL);
    ret->conns = avlmap_new();

    if (ret->hooks == NULL ||
        ret->evbase == NULL ||
        ret->htp == NULL ||
        ret->conns == NULL) {
        wshtp_server_free(ret);
        return NULL;
    }
    ret->htp->bev_flags |= BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_UNLOCK_CALLBACKS;
    return ret;
}

void wshtp_server_free(wshtp_server_t *server) {
    if (server) {
        SAFE_FREE(server->hooks);
        SAFE_FREE(server->evbase);
        SAFE_FREE(server->htp);
        SAFE_FREE(server->conns);
        free(server);
    }
}

void wshtp_ssl_init(wshtp_server_t *server, evhtp_ssl_cfg_t *scfg) {
    evhtp_ssl_use_threads();
    evhtp_ssl_init(server->htp, scfg);
}

int wshtp_server_start(wshtp_server_t *server) {
    evhtp_set_glob_cb(server->htp, "*", wshtp_handler_cb, server);
    evhtp_set_post_accept_cb(server->htp,  wshtp_post_accept_cb, server);

    int ret = event_base_loop(server->evbase, 0);
    return ret;
}

void wshtp_set_hook(wshtp_server_t *server, enum ws_hook_type_e type, wshtp_hook_cb cb, void *data) {
    wshtp_hooks_t *hooks = server->hooks;

    switch (type) {
        case WSHTP_ON_OPEN:
            wshtp_hook_set(&hooks->on_open, cb, data);
            break;
        case WSHTP_ON_CLOSE:
            wshtp_hook_set(&hooks->on_close, cb, data);
            break;
        case WSHTP_ON_GET:
            wshtp_hook_set(&hooks->on_get, cb, data);
            break;
        case WSHTP_ON_POST:
            wshtp_hook_set(&hooks->on_post, cb, data);
            break;
        case WSHTP_ON_PUT:
            wshtp_hook_set(&hooks->on_put, cb, data);
            break;
        case WSHTP_ON_DELETE:
            wshtp_hook_set(&hooks->on_delete, cb, data);
            break;
        case WSHTP_ON_MESSAGE:
            wshtp_hook_set(&hooks->on_message, cb, data);
            break;
        case WSHTP_ON_CONNECTION:
            wshtp_hook_set(&hooks->on_connection, cb, data);
            break;
    }
}

//TODO: config API

void* wshtp_conn_get_userdata(wshtp_conn_t *conn) {
    return conn->userdata;
}

void wshtp_conn_set_userdata(wshtp_conn_t *conn, void *data, wshtp_free_cb cb) {
    conn->userdata = data;
    conn->free_cb = cb;
}

int wshtp_get_data_size(wshtp_conn_t *conn) {
    return conn->data.size;
}

void *wshtp_get_data(wshtp_conn_t *conn) {
    return conn->data.content;
}

const char *wshtp_get_text(wshtp_conn_t *conn) {
    if (conn->data.type == WS_DATA_BINARY) {
        return NULL;
    }
    return conn->data.content;
}


const char *wshtp_request_get_method(wshtp_conn_t *conn) {
    return wshtp_get_method_str(conn->request->method);
}

//const char *wshtp_request_get_uri(wshtp_conn_t *conn) {
//
//}

const char *wshtp_request_get_path(wshtp_conn_t *conn) {
    return conn->request->uri->path->full;
}

const char *wshtp_request_get_query(wshtp_conn_t *conn) {
    return (char *)conn->request->uri->query_raw;
}


const char *wshtp_request_get_header(wshtp_conn_t *conn, const char *key) {
    return evhtp_header_find(conn->request->headers_in, key);
}

const char *wshtp_request_get_var(wshtp_conn_t *conn, const char *key) {
    return evhtp_kv_find(conn->request->uri->query, key);
}


void wshtp_set_code(wshtp_conn_t *conn, int code) {
    conn->reply.code = code;
}

void wshtp_add_header(wshtp_conn_t *conn, const char *key, const char *val) {
    evhtp_headers_add_header(conn->reply.headers, evhtp_header_new(key, val, 1,1));
}

void wshtp_body_add_text(wshtp_conn_t *conn, const char *data) {
    evbuffer_add(conn->reply.out, data, strlen(data));
}

void wshtp_body_add_data(wshtp_conn_t *conn, const void *data, size_t size) {
    evbuffer_add(conn->reply.out, data, size);
}

void wshtp_send_data(wshtp_conn_t *conn, const void *data, size_t size) {
    if (conn->is_websocket) {
        ws_send_frame(conn, true, OP_BIN, data, size, false, 0);
    } else {
        evbuffer_add(conn->reply.out, data, size);
        wshtp_send_reply(conn);
    }
}

void wshtp_send_text(wshtp_conn_t *conn, const char *text) {
    if (conn->is_websocket) {
        ws_send_frame(conn, true, OP_TEXT, text, strlen(text), false, 0);
    } else {
        evbuffer_add(conn->reply.out, text, strlen(text));
        wshtp_send_reply(conn);
    }
}

void wshtp_send_reply(wshtp_conn_t *conn) {
    if (conn->is_websocket) {
        return;
    } else {
        if (conn->reply.code == 0) {
            conn->reply.code = 200;
        }
        evhtp_send_reply(conn->request, conn->reply.code);
    }
}

void wshtp_send_file(wshtp_conn_t *conn, const char *filename) {
    if (conn->is_websocket) {
    } else {
        int fd = open(filename, O_RDONLY);
        if (fd >= 0) {
            off_t size = lseek(fd, 0L, SEEK_END);
            lseek(fd, 0L, SEEK_SET);
            const char * type = get_mime_type(filename);
            wshtp_add_header(conn, "Content-Type", type);
            evbuffer_add_file(conn->reply.out, fd, 0, size);
            wshtp_send_reply(conn);
            close(fd);
        }
    }
}


/* ***********************************
 *  Frame Buffer
 * ***********************************/
ws_buffer_t *ws_buffer_new() {
    ws_buffer_t *fbuf = (ws_buffer_t *)calloc(1, sizeof(ws_buffer_t));
    if (fbuf == NULL) {
        return NULL;
    }
    fbuf->head = &fbuf->root;
    return fbuf;
}

void ws_buffer_reset(ws_buffer_t *fbuf) {
    frame_node_t *next = fbuf->root;
    while(next != NULL) {
        frame_node_t *temp = next;
        next = next->next;
        free(temp->data);
        free(temp);
    }

    fbuf->root = NULL;
    fbuf->head = &fbuf->root;
}

void ws_buffer_free(ws_buffer_t *fbuf) {
    ws_buffer_reset(fbuf);
    free(fbuf);
}

void ws_buffer_append_data(ws_buffer_t *fbuf, void *data, size_t length) {
    frame_node_t *nframe = (frame_node_t*)calloc(1, sizeof(frame_node_t));
    (*fbuf->head) = nframe;
    (*fbuf->head)->data = data;
    (*fbuf->head)->length = length;
    fbuf->head = &((*fbuf->head)->next);
}

void* ws_buffer_get_data(ws_buffer_t *fbuf, size_t *length) {
    void *data = NULL;
    size_t size = 0;

    frame_node_t *next =fbuf->root;

    while (next != NULL) {
        size += next->length;
        next = next->next;
    }

    data = malloc((size)+1);

    next = fbuf->root;
    int8_t *buffer = (int8_t*)data;
    while (next != NULL) {
        memcpy(buffer, next->data, next->length);
        buffer = buffer + next->length;
        next = next->next;
    }
    *buffer = 0;
    *length = size;
    return data;
}
