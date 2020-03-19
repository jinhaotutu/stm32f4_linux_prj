/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  SYS_CFG_H
#define  SYS_CFG_H

/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Defines --------------------------------------------------------------------*/
#define LED1_ON     GPIO_ResetBits(GPIOF,GPIO_Pin_9)
#define LED1_OFF    GPIO_SetBits(GPIOF,GPIO_Pin_9)
#define LED2_ON     GPIO_ResetBits(GPIOF,GPIO_Pin_10)
#define LED2_OFF    GPIO_SetBits(GPIOF,GPIO_Pin_10)

#define LED3_ON     GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define LED3_OFF    GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LED4_ON     GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define LED4_OFF    GPIO_SetBits(GPIOC,GPIO_Pin_1)


/* Types ----------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/
void Sys_Config(void);

#endif /* SYS_CFG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
