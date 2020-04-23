/**
  *****************************************************************************
  * @file    : app_user.h
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : Header for app_user.c module
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_USER_H
#define __APP_USER_H

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __EXTERN
    #define __EXTERN
#else
    #define __EXTERN extern
#endif

/**
  *****************************************************************************
  * @version brief  :
  *****************************************************************************
  */


/* Defines --------------------------------------------------------------------*/

/* 在此设置 Freertos 的各个例程的开关 */
#define OS_TASK_LED         1
#define OS_TASK_CPU         1
#define OS_APP_SEM          0
#define OS_APP_MSG          0
#define OS_APP_MUTEX        0
#define OS_APP_TIMER        0
#define OS_APP_EVENT        0

/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif /* __APP_USER_H */

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
