/**
  *****************************************************************************
  * @file    : app_socket.c
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : socket with tcp and udp
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm_config.h"

/* Private define ------------------------------------------------------------*/
#define TCP_SERVER_EN           1
#define TCP_CLIENT_EN           0
#define UDP_SERVER_EN           1
#define UDP_CLIENT_EN           0

#define TCP_SERVER_MAX_CON      MEMP_NUM_NETCONN

#define SOCKET_DATA_SIZE        1024

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static TaskHandle_t tcp_server = NULL;
static TaskHandle_t tcp_client = NULL;
static TaskHandle_t udp_server = NULL;
static TaskHandle_t udp_client = NULL;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @note   tcp_server_cb
  * @brief  This function is used to run app task
  * @param  *p
  * @retval None
  */
static void tcp_server_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    /* 使用多路io select模型 */

    int ret = 0;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0){
        log_e("socket error");
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5550);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if(ret < 0){
        close(server_fd);
        log_e("bind error");
        return -1;
    }

    ret = listen(server_fd, 5);
    if(ret < 0){
        close(server_fd);
        log_e("listen error");
        return -1;
    }

    fd_set refset;
    int fd_max;
    int fds[TCP_SERVER_MAX_CON];

    struct timeval timeout;

    for(int i=0; i<TCP_SERVER_MAX_CON;i++){
        fds[i] = -1;
    }

    fd_max = server_fd;
    fds[0] = server_fd;

    int client_fd;
    struct sockaddr_in client_addr;
    int client_len = sizeof(struct sockaddr_in);
    bzero(&client_addr, client_len);

    uint8_t *tcp_recv_buf = pvPortMalloc(SOCKET_DATA_SIZE);
    if (tcp_recv_buf == NULL){
        close(server_fd);
        log_e("malloc error");
        return -1;
    }

    while(1){
        FD_ZERO(&refset);

        for(int i=0; i<TCP_SERVER_MAX_CON; i++)
        {
            if(fds[i] != -1)
            {
                log_d("fds[%d] = %d",i,fds[i]);
                FD_SET(fds[i],&refset);

                if(fds[i] > fd_max)
                {
                    fd_max = fds[i];
                }
            }
        }

        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        ret = select(fd_max+1, &refset, NULL, NULL, &timeout);
        switch(ret){
            case 0:
                // log_d("select timeout!");
                break;

            case -1:
                log_e("select error!");
                break;

            default:
                for(int i=0;i<TCP_SERVER_MAX_CON;i++){
                    if (fds[i] == server_fd && FD_ISSET(server_fd, &refset)){
                        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
                        if (client_fd < 0){
                            close(server_fd);
                            log_e("accept error");
                            return -1;
                        }

                        uint8_t *addr = inet_ntoa(client_addr.sin_addr);
                        uint16_t port = ntohs(client_addr.sin_port);
                        log_i("client ip : %s , port : %d", addr, port);

                        for(int j=0; j<TCP_SERVER_MAX_CON;j++){
                            if(fds[j] == -1){
                                fds[j] = client_fd;
                                break;
                            }
                        }
                    }else if (fds[i] >= 0 && FD_ISSET(fds[i], &refset)){
                        memset(tcp_recv_buf, 0, SOCKET_DATA_SIZE);
                        int num = recv(fds[i], tcp_recv_buf, SOCKET_DATA_SIZE, 0);
                        if(num <= 0){
                            close(fds[i]);
                            fds[i] = -1;
                            log_d("closed");
                            break;
                        }

                        log_i("recv data:%s", tcp_recv_buf);

                        num = send(fds[i], tcp_recv_buf, num, 0);
                        if (num >= 0){
                            log_i("send succeed");
                        }
                    }
                }
                break;
        }
    }

    vPortFree(tcp_recv_buf);
    close(server_fd);
}

/**
  * @note   tcp_server_cb
  * @brief  This function is used to run app task
  * @param  *p
  * @retval None
  */
static void tcp_client_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    int ret = 0;
    int client_fd;

    while(1){
        client_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (client_fd < 0){
            log_e("socket error");
            return -1;
        }

        struct sockaddr_in server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5551);
        server_addr.sin_addr.s_addr = inet_addr("192.168.1.66");

        uint8_t *recv_buf = pvPortMalloc(SOCKET_DATA_SIZE);
        if (recv_buf == NULL){
            log_e("malloc error");
            close(client_fd);
            continue;
        }

        while(1){
            ret = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
            if (ret < 0){
                log_e("connect error");
                break;
            }

            log_i("connect succeed");

            while(1){
                vTaskDelay(1000);

                uint8_t *msg = "tcp client -----> hello world!";
                int num = send(client_fd, msg, strlen(msg), 0);
                if (num < 0){
                    log_e("send error");
                    break;
                }

                log_i("client send succeed");

                memset(recv_buf, 0, SOCKET_DATA_SIZE);

                num = recv(client_fd, recv_buf, SOCKET_DATA_SIZE, 0);
                if (num < 0){
                    log_e("recv error");
                    break;
                }else if(num == 0){
                    log_d("server closed");
                    break;
                }else{
                    log_i("tcp server ----->%s", recv_buf);
                }
            }

            break;
        }

        vPortFree(recv_buf);
        recv_buf = NULL;
        close(client_fd);
        vTaskDelay(1000);
    }
}

/**
  * @note   udp_server_cb
  * @brief  This function is used to run app task
  * @param  *p
  * @retval None
  */
static void udp_server_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    int ret = 0;

    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0){
        log_e("socket error");
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5553);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if(ret < 0){
        close(server_fd);
        log_e("bind error");
        return -1;
    }

    fd_set refset;
    struct timeval timeout;

    int client_fd;
    struct sockaddr_in client_addr;
    int client_len = sizeof(struct sockaddr_in);
    bzero(&client_addr, client_len);

    uint8_t *recv_buf  = pvPortMalloc(SOCKET_DATA_SIZE);
    if (recv_buf == NULL){
        close(server_fd);
        log_e("malloc error");
        return -1;
    }

    while(1){
        FD_ZERO(&refset);
        FD_SET(server_fd, &refset);

        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        ret = select(server_fd+1, &refset, NULL, NULL, &timeout);
        switch(ret){
            case 0:
                // log_d("select timeout!");
                break;

            case -1:
                log_e("select error!");
                break;

            default:
                memset(recv_buf, 0, SOCKET_DATA_SIZE);
                int num = recvfrom(server_fd, recv_buf, SOCKET_DATA_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
                if(num < 0){
                    close(server_fd);
                    break;
                }

                uint8_t *addr = inet_ntoa(client_addr.sin_addr);
                uint16_t port = ntohs(client_addr.sin_port);
                log_i("client ip : %s , port : %d", addr, port);
                log_i("recv data:%s", recv_buf);

                num = sendto(server_fd, recv_buf, num, 0, (struct sockaddr*)&client_addr, client_len);
                if (num >= 0){
                    log_i("send succeed");
                }

                break;
        }
    }

    close(server_fd);
}

/**
  * @note   udp_client_cb
  * @brief  This function is used to run app task
  * @param  *p
  * @retval None
  */
static void udp_client_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    int ret = 0;
    int client_fd;

    while(1){
        client_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (client_fd < 0){
            log_e("socket error");
            return -1;
        }

        int optval = 1;
        setsockopt(client_fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

        struct sockaddr_in server_addr;
        int server_len = sizeof(struct sockaddr_in);
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5554);
        server_addr.sin_addr.s_addr = htonl(IPADDR_BROADCAST);//inet_addr("192.168.1.66");

        uint8_t *recv_buf = pvPortMalloc(SOCKET_DATA_SIZE);
        if (recv_buf == NULL){
            close(client_fd);
            log_e("malloc error");
            continue;
        }

        while(1){
            vTaskDelay(1000);

            uint8_t *msg = "udp client -----> hello world!\r\n";
            int num = sendto(client_fd, msg, strlen(msg), 0, (struct sockaddr*)&server_addr, server_len);
            if (num < 0){
                log_e("send error");
                break;
            }

            log_i("client send succeed");

            memset(recv_buf, 0, SOCKET_DATA_SIZE);

            num = recvfrom(client_fd, recv_buf, SOCKET_DATA_SIZE, 0, (struct sockaddr*)&server_addr, &server_len);
            if (num < 0){
                log_e("recv error");
                break;
            }else{
                log_i("udp server ----->%s", recv_buf);
            }
        }

        vPortFree(recv_buf);
        recv_buf = NULL;
        close(client_fd);
        vTaskDelay(1000);
    }
}

/**
  * @note   app_socket_start
  * @brief  None
  * @param  None
  * @retval int
  */
int app_socket_start(void)
{
    BaseType_t xReturn = pdPASS;

    while(get_net_status() == 0);

#if TCP_SERVER_EN
    xReturn = xTaskCreate(  (TaskFunction_t )tcp_server_cb,
                            (const char *   )"tcp_server",
                            (unsigned short )1024,
                            (void *         )NULL,
                            (UBaseType_t    )RTOS_PRIORITY_LEVEL_2ST,
                            (TaskHandle_t * )&tcp_server);

    if (pdPASS != xReturn){
        return pdFALSE;
    }
#endif

#if TCP_CLIENT_EN
    xReturn = xTaskCreate(  (TaskFunction_t )tcp_client_cb,
                            (const char *   )"tcp_client",
                            (unsigned short )1024,
                            (void *         )NULL,
                            (UBaseType_t    )RTOS_PRIORITY_LEVEL_2ST,
                            (TaskHandle_t * )&tcp_client);

    if (pdPASS != xReturn){
        return pdFALSE;
    }
#endif

#if UDP_SERVER_EN
    xReturn = xTaskCreate(  (TaskFunction_t )udp_server_cb,
                            (const char *   )"udp_server",
                            (unsigned short )1024,
                            (void *         )NULL,
                            (UBaseType_t    )RTOS_PRIORITY_LEVEL_2ST,
                            (TaskHandle_t * )&udp_server);

    if (pdPASS != xReturn){
        return pdFALSE;
    }
#endif

#if UDP_CLIENT_EN
    xReturn = xTaskCreate(  (TaskFunction_t )udp_client_cb,
                            (const char *   )"tcp_client",
                            (unsigned short )1024,
                            (void *         )NULL,
                            (UBaseType_t    )RTOS_PRIORITY_LEVEL_2ST,
                            (TaskHandle_t * )&udp_client);

    if (pdPASS != xReturn){
        return pdFALSE;
    }
#endif

    return pdPASS;
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
