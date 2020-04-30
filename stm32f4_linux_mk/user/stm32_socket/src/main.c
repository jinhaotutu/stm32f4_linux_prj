/**
  *****************************************************************************
  * @file    : main.c
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : stm32f407 socket工程
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "stm_config.h"

/* Defines --------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/
static TaskHandle_t xTask_creat;

/* Functions ------------------------------------------------------------------*/

/**
  * @note   This function is used to creat app task and delect self.
  * @brief  None
  * @param  *p
  * @retval None
  */
static void creat_task(void *p)
{
    log_d("%s", __FUNCTION__);

    /* creat app task in this 在这里创建应用任务 */
    taskENTER_CRITICAL();

    app_task_init();

    taskEXIT_CRITICAL();
    /* creat app task in this 在这里创建应用任务 */

    log_d("delete creat task");

    vTaskDelete(xTask_creat);
}

/**
  * @note   rtos_init.
  * @brief  None
  * @param  *p
  * @retval None
  */
void rtos_init(void)
{
    BaseType_t xReturn = pdPASS;

    log_i("Freertos v10.2.1 start ");

    /* first creat task in this 创建rtos第一个任务，用于创建其他任务 */
    xReturn = xTaskCreate(  (TaskFunction_t )creat_task,
                            (const char *   )"creat_task",
                            (unsigned short )1024,
                            (void *         )NULL,
                            (UBaseType_t    )RTOS_PRIORITY_HIGHEST,
                            (TaskHandle_t * )&xTask_creat);

    if (pdPASS != xReturn){
        return -1;
    }

    /* start task 开启任务调度 */
    vTaskStartScheduler();
}

/**
  * @note   Main program
  * @brief  None
  * @param  *p
  * @retval None
  */
int main(void)
{
    /* stm32系统配置 */
    Sys_Config();

    log_i("sys config finish");

#ifdef USE_OS
    rtos_init();
#endif

    log_e("never get here");

    while(1){
    }
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/

