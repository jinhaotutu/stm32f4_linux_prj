/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 *
 * This program fetches HTTP URLs.
 */

#include "mongoose.h"
#include "stm_config.h"

static int s_exit_flag = 0;
static int s_show_headers = 0;
static const char *s_show_headers_opt = "--show-headers";

static struct mg_mgr http_mgr;
static TaskHandle_t http_task = NULL;
static uint8_t recv_data[128];

static void recv_handler(struct mg_connection *nc, int ev, void *ev_data)
{
    struct http_message *hm = (struct http_message *) ev_data;

    switch (ev) {
        case MG_EV_CONNECT:
            if (*(int *) ev_data != 0) {
                fprintf(stderr, "connect() failed: %s\r\n", strerror(*(int *) ev_data));
                s_exit_flag = 1;
                printf("%s", stderr);
            }
            printf("Server connect succeed\r\n");
            break;

        case MG_EV_HTTP_REPLY:
            nc->flags |= MG_F_CLOSE_IMMEDIATELY;
            if (s_show_headers) {
                fwrite(hm->message.p, 1, hm->message.len, stdout);
            } else {
                fwrite(hm->body.p, 1, hm->body.len, stdout);
            }
            putchar('\n');
            s_exit_flag = 1;
            printf("Server reply :%d\r\n", hm->resp_code);
            memset(recv_data, 0, 128);
            memcpy(recv_data, hm->body.p, hm->body.len);
            printf("recv data: %s\r\n", recv_data);
            break;

        case MG_EV_CLOSE:
            if (s_exit_flag == 0) {
                printf("Server closed connection\r\n");
                s_exit_flag = 1;
            }
            break;

        default:
            break;
    }
}

static void http_task_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    while(1) {
        mg_mgr_poll(&http_mgr, 1000);
        vTaskDelay(200);
    }

    mg_mgr_free(&http_mgr);
}

int http_common_init(void)
{
    mg_mgr_init(&http_mgr, NULL);

    BaseType_t xReturn = pdPASS;

    xReturn = xTaskCreate(  (TaskFunction_t )http_task_cb,
                            (const char *   )"http_task",
                            (unsigned short )1024,
                            (void *         )NULL,
                            (UBaseType_t    )RTOS_PRIORITY_LEVEL_2ST,
                            (TaskHandle_t * )&http_task);

    if (pdPASS != xReturn){
        return -1;
    }
}

int http_common_post(uint8_t *url, uint8_t *post_date)
{
    int ret;

    mg_connect_http(&http_mgr, recv_handler, url, NULL, post_date);

    return 0;
}
