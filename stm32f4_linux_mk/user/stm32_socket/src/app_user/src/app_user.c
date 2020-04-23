/**
  *****************************************************************************
  * @file    : task_app.c
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : task function
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

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static TaskHandle_t task_led = NULL;

static TaskHandle_t task_tcp = NULL;
static TaskHandle_t task_udp = NULL;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @note   task_led_cb
  * @brief  This function is used to run app task
  * @param  *p
  * @retval None
  */
static void task_led_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    while(1){
        log_d("led on...");
        LED1_ON;
        vTaskDelay(1000);

        log_d("led off...");
        LED1_OFF;
        vTaskDelay(1000);
    }
}

uint8_t buff[1024];

/**
  * @note   task_tcp_cb
  * @brief  This function is used to run app task
  * @param  *p
  * @retval None
  */
static void task_tcp_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in my_addr;
    my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(8888);
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero),8);

    bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr));

    listen(sockfd, 5);

    struct sockaddr_in their_addr;
    int len = sizeof(struct sockaddr_in);
    int numbytes = 0;

    while(1){
        int new_fd = accept(sockfd,(struct sockaddr*)&their_addr, &len);

        numbytes = send(new_fd,"Welcome to my server\n",21,0);

        while((numbytes = recv(new_fd, buff, 1024, 0)) > 0)
        {
            buff[numbytes] = '\0';
            log_i("%s\n",buff);
            send(new_fd,buff,numbytes,0);
        }
    }
}

/**
  * @note   app_user_main
  * @brief  None
  * @param  None
  * @retval int
  */
void app_user_main(void)
{
    /* app task in this 应用入口封装函数 */

    BaseType_t xReturn = pdPASS;

    xReturn = xTaskCreate(  (TaskFunction_t )task_led_cb,
                            (const char *   )"task_led",
                            (unsigned short )256,
                            (void *         )NULL,
                            (UBaseType_t    )1,
                            (TaskHandle_t * )&task_led);

    if (pdPASS != xReturn){
        return -1;
    }

    xReturn = xTaskCreate(  (TaskFunction_t )task_tcp_cb,
                            (const char *   )"task_tcp",
                            (unsigned short )1024,
                            (void *         )NULL,
                            (UBaseType_t    )1,
                            (TaskHandle_t * )&task_tcp);

    if (pdPASS != xReturn){
        return -1;
    }

    return 0;
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
