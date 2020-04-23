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
static TaskHandle_t task_app = NULL;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @note   task_led_cb
  * @brief  This function is used to run app task
  * @param  *p
  * @retval None
  */
static void app_user(void *p)
{
    log_d("%s init", __FUNCTION__);

    extern void app_user_main(void);
    app_user_main();

    vTaskDelete(task_app);
}

/**
  * @note   app_task_init
  * @brief  None
  * @param  None
  * @retval int
  */
int app_task_init(void)
{
    BaseType_t xReturn = pdPASS;

    xReturn = NetWork_Init();
    if (pdPASS != xReturn){
        return -1;
    }

    /* app task in this 创建app应用任务 */
    xReturn = xTaskCreate(  (TaskFunction_t )app_user,
                            (const char *   )"app_user",
                            (unsigned short )2048,
                            (void *         )NULL,
                            (UBaseType_t    )1,
                            (TaskHandle_t * )&task_app);

    if (pdPASS != xReturn){
        return -1;
    }

    return 0;
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
