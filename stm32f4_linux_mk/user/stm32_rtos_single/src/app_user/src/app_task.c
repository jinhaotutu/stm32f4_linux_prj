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
        vTaskDelay(2000);

        log_d("led off...");
        LED1_OFF;
        vTaskDelay(2000);
    }
}

/**
  * @note   led_task_init
  * @brief  None
  * @param  None
  * @retval int
  */
BaseType_t led_task_init(void)
{
    BaseType_t xReturn = pdPASS;

    log_d("led task creat");

    /* app task in this 创建rtos应用任务 */
    xReturn = xTaskCreate(  (TaskFunction_t )task_led_cb,
                            (const char *   )"task_led",
                            (unsigned short )256,
                            (void *         )NULL,
                            (UBaseType_t    )1,
                            (TaskHandle_t * )&task_led);

    if (pdPASS != xReturn){
        return pdFAIL;
    }

    return pdPASS;
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
