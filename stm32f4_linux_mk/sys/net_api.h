/**
  *****************************************************************************
  * @file    : network.h
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : Header for network config
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  NET_API_H
#define  NET_API_H

/* Includes -------------------------------------------------------------------*/


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


/* Types ----------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/
uint32_t NetWork_Init(void);
void LwIP_Pkt_Handle(void);
void LwIP_Periodic_Handle(void);


#ifdef __cplusplus
}
#endif

#endif /* NET_API_H */

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/

