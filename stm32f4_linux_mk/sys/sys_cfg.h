/**
  *****************************************************************************
  * @file    : sys_cfg.h
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : Header for sys_cfg config
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  SYS_CFG_H
#define  SYS_CFG_H

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

/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Defines --------------------------------------------------------------------*/
#define LED1_ON     GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define LED1_OFF    GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LED2_ON     GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define LED2_OFF    GPIO_SetBits(GPIOD,GPIO_Pin_3)


/* Types ----------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/

/**
  * @note   Sys_Config
  * @brief  None
  * @param  None
  * @retval None
  */
void Sys_Config(void);

/**
  * @note   delay_init
  * @brief  None
  * @param  nTime
  * @retval None
  */
void delay_init(void);

/**
  * @note   delay_ms
  * @brief  None
  * @param  nTime : range is 0-32767
  * @retval None
  */
void delay_ms(uint32_t nTime);


/**
  * @note   delay_us
  * @brief  None
  * @param  nTime : range is 0-32767000
  * @retval None
  */
void delay_us(uint32_t nTime);

/**
  * @note   thirdlib_init
  * @brief  None
  * @param  None
  * @retval None
  */
void thirdlib_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SYS_CFG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
