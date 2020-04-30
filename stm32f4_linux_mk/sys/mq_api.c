/* Standard includes. */
#include "mq_api.h"

#include "stm_config.h"

#define MQTT_BUF_SIZE       1024

int mqtt_client_start(MQ_DEFS* mq_params)
{
    int ret = -1;

    NetworkInit(&(mq_params->network));

    mq_params->send_buf = pvPortMalloc(MQTT_BUF_SIZE);
    if (NULL == mq_params->send_buf){
        return -1;
    }
    memset(mq_params->send_buf, 0, MQTT_BUF_SIZE);

    mq_params->recv_buf = pvPortMalloc(MQTT_BUF_SIZE);
    if (NULL == mq_params->recv_buf){
        return -1;
    }
    memset(mq_params->recv_buf, 0, MQTT_BUF_SIZE);

    MQTTClientInit(&(mq_params->client), &(mq_params->network), 30000, mq_params->send_buf, MQTT_BUF_SIZE, mq_params->recv_buf, MQTT_BUF_SIZE);

    ret = NetworkConnect(&(mq_params->network), mq_params->mq_url, mq_params->port);
    if (ret != 0){
        log_e("Return code from network connect is %d\n", ret);
        return ret;
    }

    ret = MQTTConnect(&(mq_params->client), &(mq_params->con));
    if (ret != 0){
        log_e("Return code from MQTT connect is %d\n", ret);
        return ret;
    }

    log_i("MQTT Connected\n");

    return 0;
}

int mqtt_client_stop(MQ_DEFS* mq_params)
{
    vPortFree(mq_params->send_buf);
    mq_params->send_buf = NULL;
    vPortFree(mq_params->recv_buf);
    mq_params->recv_buf = NULL;
}

int mqtt_sub_topic(MQTTClient *client, uint8_t *topic, messageHandler recv_cb)
{
    int ret = -1;

    ret = MQTTSubscribe(client, topic, 1, recv_cb);
    if (ret != 0){
        log_e("Return code from MQTT subscribe is %d\n", ret);
        return ret;
    }

    log_i("subscribe [%s] succeed", topic);

    return 0;
}

int mqtt_pub_topic(MQTTClient *client, uint8_t *topic, MQTTMessage *pub_data)
{
    int ret = -1;

    ret = MQTTPublish(client, topic, pub_data);
    if (ret != 0){
        log_e("Return code from MQTT publish is %d\n", ret);
        return ret;
    }

    log_i("publish [%s] succeed", topic);

    return 0;
}


/*-----------------------------------------------------------*/


