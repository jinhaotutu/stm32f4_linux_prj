/**
  *****************************************************************************
  * @file    : main.c
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : stm32f407+lwip+no_os 移植例程
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "stm_config.h"

#include "network.h"
#include "net_api.h"

/* Defines --------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/
static __IO uint32_t LocalTime = 0;

/* Functions ------------------------------------------------------------------*/

void update_lwip_tick(uint32_t tick)
{
    LocalTime += tick;
}

/**
  * @note   Main program
  * @brief  None
  * @param  *p
  * @retval None
  */
int main(void)
{
    /* stm32系统配置 */
    Sys_Config();

    log_d("sys config finish");

    NetWork_Init();

    LED1_ON;

    while(1)
    {
        if (ETH_CheckFrameReceived())
        {
            /* process received ethernet packet*/
            LwIP_Pkt_Handle();
        }

        /* handle periodic timers for LwIP*/
        LwIP_Periodic_Handle(LocalTime);
    }
}

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/

