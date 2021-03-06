/**
  *****************************************************************************
  * @file    : stm_config.h
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : Header for stm32f4 config
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  STM32_CONFIG_H
#define  STM32_CONFIG_H

/* Includes -------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "sys_cfg.h"

/* third lib include */
#include <elog.h>

/* rtos */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"
#include "timers.h"

/* lwip */
#include "stm32f4x7_eth.h"
#include "lwipopts.h"
#include "lwip/sockets.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __EXTERN_GLOBAL
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
#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#endif
#define os_printf(_fmt_, ...) \
        printf("[log ] %s:%d | "_fmt_"\r\n", __FILENAME__, __LINE__, ##__VA_ARGS__)

/* user define */
#define RTOS_PRIORITY_HIGHEST       (10)
#define RTOS_PRIORITY_LEVEL_1ST     (9)
#define RTOS_PRIORITY_LEVEL_2ST     (8)
#define RTOS_PRIORITY_LEVEL_3ST     (7)
#define RTOS_PRIORITY_LEVEL_4ST     (6)
#define RTOS_PRIORITY_LEVEL_5ST     (5)

/* Types ----------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* STM32_CONFIG_H */

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/
