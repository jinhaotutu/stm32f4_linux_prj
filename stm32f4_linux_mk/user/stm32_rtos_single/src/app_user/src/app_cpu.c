/**
  *****************************************************************************
  * @file    : task_cpu.c
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
static TaskHandle_t task_cpu  = NULL;
static uint8_t CPU_info[1024] = {0};

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @note   task_cpu_cb
  * @brief  None
  * @param  *p
  * @retval None
  */
static void task_cpu_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    while(1){
        vTaskDelay(5000);

        memset(CPU_info, 0, 1024);

        log_raw("\r\n------------- cpu info -------------\r\n");

        vTaskList((char *)CPU_info);
        log_raw("\r\nname        state  priority  stack   num\r\n");
        log_raw("%s", CPU_info);

#if (defined configGENERATE_RUN_TIME_STATS) && (configGENERATE_RUN_TIME_STATS == 1)
        memset(CPU_info, 0, 1024);
        vTaskGetRunTimeStats((char *)CPU_info);
        log_raw("\r\nname          cnt            used\r\n");
        log_raw("%s\r\n", CPU_info);
#endif
        log_raw("------------------------------------\r\n\r\n");
    }
}

/**
  * @note   cpu_task_init
  * @brief  None
  * @param  None
  * @retval None
  */
BaseType_t cpu_task_init(void)
{
    BaseType_t xReturn = pdPASS;

    log_d("cpu task creat");

    xReturn = xTaskCreate(  (TaskFunction_t )task_cpu_cb,
                            (const char *   )"task_cpu",
                            (unsigned short )512,
                            (void *         )NULL,
                            (UBaseType_t    )1,
                            (TaskHandle_t * )&task_cpu);

    if (pdPASS != xReturn){
        return pdFAIL;
    }

    return pdPASS;
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
