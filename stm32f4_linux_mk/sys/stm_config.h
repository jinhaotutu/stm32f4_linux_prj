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

/* Defines --------------------------------------------------------------------*/
#define os_printf(_fmt_, ...)   log_i(_fmt_, ##__VA_ARGS__)

/* user system define */
#define     USE_OS          0

/* Types ----------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/

#endif /* STM32_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
