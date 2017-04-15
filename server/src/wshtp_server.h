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

#ifndef __WSHTP_SERVER_H__
#define __WSHTP_SERVER_H__
#include <evhtp/evhtp.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "wshtp_avl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ws_buffer_s ws_buffer_t;

typedef struct wshtp_conn_s wshtp_conn_t;
typedef struct wshtp_server_s wshtp_server_t;
typedef struct wshtp_hooks_s wshtp_hooks_t;
typedef struct wshtp_cfg_s wshtp_cfg_t;

typedef int (*wshtp_hook_cb)(wshtp_conn_t *conn, void *data);
typedef void (*wshtp_free_cb)(wshtp_conn_t *conn, void *data);


enum ws_hook_type_e {
    WSHTP_ON_OPEN = 1,
    WSHTP_ON_CLOSE,
    WSHTP_ON_GET,
    WSHTP_ON_POST,
    WSHTP_ON_PUT,
    WSHTP_ON_DELETE,
    WSHTP_ON_MESSAGE,
    WSHTP_ON_CONNECTION
};

enum ws_data_type_e {
    WS_DATA_BINARY = 0,
    WS_DATA_TEXT
};

struct wshtp_conn_s {
    wshtp_server_t *server;
    evhtp_request_t *request;
    evhtp_connection_t *conn;

    bool is_websocket;

    struct {
        enum ws_data_type_e type;
        char *content;
        size_t size;
        ws_buffer_t *ws_frames;
    } data;

    struct {
        evhtp_res code;
        evhtp_headers_t *headers;
        evbuf_t *out;
    } reply;

    void *userdata;
    wshtp_free_cb free_cb;

    enum ws_hook_type_e method;
};

struct wshtp_server_s {
    evbase_t *evbase;
    evhtp_t *htp;

    avlmap_t *conns;

    wshtp_hooks_t *hooks;
};

struct wshtp_cfg_s {
    const char *addr;
    uint16_t port;
    int backlog;

    //SSL config
    evhtp_ssl_cfg_t *ssl_cfg;
};

/* *****************************
 * Public API
 * *****************************/
wshtp_server_t *wshtp_server_new(void);

void wshtp_server_free(wshtp_server_t *server);
void wshtp_ssl_init(wshtp_server_t *server, evhtp_ssl_cfg_t *scfg);

int wshtp_server_start(wshtp_server_t *server);

void wshtp_set_hook(wshtp_server_t *server, enum ws_hook_type_e type, wshtp_hook_cb, void *data);

void *wshtp_conn_get_userdata(wshtp_conn_t *conn);
void wshtp_conn_set_userdata(wshtp_conn_t *conn, void *data, wshtp_free_cb cb);

int wshtp_get_data_size(wshtp_conn_t*conn);
void *wshtp_get_data(wshtp_conn_t*conn);
const char *wshtp_get_text(wshtp_conn_t *conn);

const char *wshtp_request_get_method(wshtp_conn_t *conn);
const char *wshtp_request_get_path(wshtp_conn_t *conn);
const char *wshtp_request_get_query(wshtp_conn_t *conn);

const char *wshtp_request_get_header(wshtp_conn_t *conn, const char *key);
const char *wshtp_request_get_var(wshtp_conn_t *conn, const char *key);


void wshtp_set_code(wshtp_conn_t *conn, int code);
void wshtp_add_header(wshtp_conn_t *conn, const char *key, const char *val);

void wshtp_body_add_text(wshtp_conn_t *conn, const char *text);
void wshtp_body_add_data(wshtp_conn_t *conn, const void *data, size_t size);

void wshtp_send_data(wshtp_conn_t *conn, const void *data, size_t size);
void wshtp_send_text(wshtp_conn_t *conn, const char *text);

void wshtp_send_file(wshtp_conn_t *conn, const char *filename);

void wshtp_send_reply(wshtp_conn_t *conn);

#ifdef __cplusplus
}
#endif

#endif
