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
#ifndef  MQ_API_H
#define  MQ_API_H

/* Includes -------------------------------------------------------------------*/
#include "MQTTClient.h"
#include "MQTTConnect.h"

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
typedef struct{
    /* data */
    uint8_t *mq_url;
    uint16_t port;
    uint8_t *send_buf;
    uint8_t *recv_buf;
    MQTTPacket_connectData con;
    MQTTClient client;
    Network network;
}MQ_DEFS;

/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/
int mqtt_client_start(MQ_DEFS* mq_params);
int mqtt_client_stop(MQ_DEFS* mq_params);
int mqtt_sub_topic(MQTTClient *client, uint8_t *topic, messageHandler recv_cb);
int mqtt_pub_topic(MQTTClient *client, uint8_t *topic, MQTTMessage *pub_data);

#ifdef __cplusplus
}
#endif

#endif /* MQ_API_H */

/************************ (C) COPYRIGHT Tuu ********END OF FILE****************/

