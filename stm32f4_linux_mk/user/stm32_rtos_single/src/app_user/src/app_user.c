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
#include "app_user.h"

/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @note   app_user_main
  * @brief  None
  * @param  None
  * @retval int
  */
BaseType_t app_user_main(void)
{
    /* app task in this 应用入口封装函数 */

    BaseType_t xReturn = pdPASS;

#if OS_TASK_LED
    xReturn = led_task_init();
    if (pdTRUE != xReturn){
        log_e("init err");
        return xReturn;
    }
#endif

#if OS_TASK_CPU
    xReturn = cpu_task_init();
    if (pdTRUE != xReturn){
        log_e("init err");
        return xReturn;
    }
#endif

#if OS_APP_SEM
    xReturn = sem_init();
    if (pdTRUE != xReturn){
        log_e("init err");
        return xReturn;
    }
#endif

#if OS_APP_MSG
    xReturn = message_init();
    if (pdTRUE != xReturn){
        log_e("init err");
        return xReturn;
    }
#endif

#if OS_APP_MUTEX
    xReturn = mutex_init();
    if (pdTRUE != xReturn){
        log_e("init err");
        return xReturn;
    }
#endif

#if OS_APP_TIMER
    xReturn = timer_init();
    if (pdTRUE != xReturn){
        log_e("init err");
        return xReturn;
    }
#endif

#if OS_APP_EVENT
    xReturn = event_init();
    if (pdTRUE != xReturn){
        log_e("init err");
        return xReturn;
    }
#endif

    return pdPASS;
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
