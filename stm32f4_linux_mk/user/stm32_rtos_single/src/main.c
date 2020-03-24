/**
  ******************************************************************************
  * @file    main.c
  * @author  jinhao
  * @version V1.0.0
  * @date    2020/03/23
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "main.h"
#include "stm_config.h"

/* Defines --------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/
static TaskHandle_t xTask_creat;

/* Functions ------------------------------------------------------------------*/

/**
  * @brief  freertos first creat task
  * @note   This function is used to creat app task and delect self.
  * @param  *p
  * @param  None
  * @retval None
  */
static void creat_task(void *p)
{
    os_printf("%s", __FUNCTION__);
    int cnt = 0;

    /* creat app task in this 在这里创建应用任务 */
    // taskENTER_CRITICAL();

    // extern int app_task_init(void);
    // app_task_init();

    // taskEXIT_CRITICAL();
    /* creat app task in this 在这里创建应用任务 */

    /* delay task 延时退出，并删除本任务 */
    while(1){
        os_printf("this is creat task:idle-%d", cnt++);
        vTaskDelay(1000);

        if (cnt >= 10){
            break;
        }
    }

    os_printf("delete creat task");

    vTaskDelete(xTask_creat);
}

void rtos_init(void)
{
    BaseType_t xReturn = pdPASS;

    os_printf("Freertos v10.2.1 start ");

    /* first creat task in this 创建rtos第一个任务，用于创建其他任务 */
    xReturn = xTaskCreate(  (TaskFunction_t )creat_task,
                            (const char *   )"creat_task",
                            (unsigned short )256,
                            (void *         )NULL,
                            (UBaseType_t    )1,
                            (TaskHandle_t * )&xTask_creat);

    if (pdPASS != xReturn){
        return -1;
    }

    /* start task 开启任务调度 */
    vTaskStartScheduler();
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    /* stm32系统配置 */
    Sys_Config();

    log_d("sys config finish");

    rtos_init();

    while(1)
    {
        log_d("never get there");
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
