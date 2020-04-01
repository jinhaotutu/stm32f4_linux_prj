/**
  ******************************************************************************
  * @file    task_cpu.c
  * @author  Tuu
  * @version V1.0.0
  * @date    2020-01-28
  * @brief   task function
  ******************************************************************************
  * @attention
  * Freertos run in the linux
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm_config.h"

/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static TaskHandle_t task_cpu  = NULL;
static uint8_t CPU_info[1024] = {0};

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  task_cpu_cb
  * @note   None
  * @param  *p
  * @param  None
  * @retval None
  */
static void task_cpu_cb(void *p)
{
    log_d("%s", __FUNCTION__);

    while(1){
        vTaskDelay(5000);

        memset(CPU_info, 0, 1024);

        vTaskList((char *)CPU_info);
        log_raw("\r\n------------- cpu info -------------\r\n");
        log_raw("\r\nname            p       p        x      t\r\n");
        log_raw("%s", CPU_info);

        memset(CPU_info, 0, 1024);
        vTaskGetRunTimeStats((char *)CPU_info);
        log_raw("\r\nname            cnt            used\r\n");
        log_raw("%s\r\n", CPU_info);
        log_raw("------------------------------------\r\n\r\n");
    }
}

/**
  * @brief  cpu_task_init
  * @note   None
  * @param  None
  * @param  None
  * @retval None
  */
int cpu_task_init(void)
{
    BaseType_t xReturn = pdPASS;

    xReturn = xTaskCreate(  (TaskFunction_t )task_cpu_cb,
                            (const char *   )"task_cpu",
                            (unsigned short )512,
                            (void *         )NULL,
                            (UBaseType_t    )1,
                            (TaskHandle_t * )&task_cpu);

    if (pdPASS != xReturn){
        return -1;
    }

    return 0;
}


/**
  * @}
  */

/******************************** (C) Tuu *********************END OF FILE****/