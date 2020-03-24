/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  STM32_CONFIG_H
#define  STM32_CONFIG_H

/* Includes -------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "sys_cfg.h"
#include "sys_delay.h"

/* third lib include */
#include <elog.h>

/* rtos */
#include "FreeRTOS.h"
#include "task.h"

/* Defines --------------------------------------------------------------------*/
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#define os_printf(_fmt_, ...) \
        printf("[log ] %s:%d | "_fmt_"\r\n", __FILENAME__, __LINE__, ##__VA_ARGS__)


/* user system define */
#define USE_OS          1

/* Types ----------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/

#endif /* STM32_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
