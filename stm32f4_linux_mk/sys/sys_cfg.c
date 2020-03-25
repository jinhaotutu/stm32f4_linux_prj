/**
  ******************************************************************************
  * @file    sys_delay.c
  * @author  jinhao
  * @version V1.0.0
  * @date    22-April-2016
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

/* Includes ------------------------------------------------------------------*/
#include "stm_config.h"
#include "sys_delay.h"


/* Defines --------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/


/***
 * 函数名称 : RCC_Configuration();
 *
 * 函数描述 : RCC系统时钟初始化配置;
 *
 * 传递值	: 无;
 *
 * 返回值   : 无;
 *
 **/
static void RCC_Configuration(void)
{
    RCC_DeInit();

    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_WaitForHSEStartUp() != SUCCESS);

    /*
     * PLL_N = 336
     * PLL_M = 8
     * PLL_P = 2
     *
     * PLLP = 晶振 * PLL_N (PLL_M * PLL_P) = 8MHz * 336 / (8 * 2) = 168MHz
     */
    RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08){}

    RCC_HCLKConfig(RCC_HCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div4);
    RCC_PCLK2Config(RCC_HCLK_Div2);
}

/***
 * 函数名称 : GPIO_Configuration();
 *
 * 函数描述 : GPIO初始化配置;
 *
 * 传递值	: 无;
 *
 * 返回值   : 无;
 *
 **/
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	  

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

    /* LED GPIO口配置 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_0);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);

    /* USART1 GPIO口配置 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
}

/***
 * 函数名称 : NVIC_Configuration();
 *
 * 函数描述 : 中断初始化配置;
 *
 * 传递值	: 无;
 *
 * 返回值   : 无;
 *
 **/
static void NVIC_Configuration(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
}

/***
 * 函数名称 : USART_Configuration();
 *
 * 函数描述 : 串口初始化配置;
 *
 * 传递值	: 无;
 *
 * 返回值   : 无;
 *
 **/
static void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1,ENABLE);
}

/***
 * 函数名称 : SysTick_Configuration();
 *
 * 函数描述 : tick初始化配置;
 *
 * 传递值	: 无;
 *
 * 返回值   : 无;
 *
 **/
static void SysTick_Configuration(void)
{
    u32 reload;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	// fac_us=SystemCoreClock;							//不论是否使用OS,fac_us都需要使用
	reload=configCPU_CLOCK_HZ/configTICK_RATE_HZ;							//每秒钟的计数次数 单位为M	   
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	
	// fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/configTICK_RATE_HZ断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK   
}

/***
 * 函数名称 : easy_logger_init();
 *
 * 函数描述 : 日志初始化配置;
 *
 * 传递值	: 无;
 *
 * 返回值   : 无;
 *
 **/
static void easy_logger_init(void)
{
    printf("\r\n\r\n");
    /* initialize EasyLogger */
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL);
    /* start EasyLogger */
    elog_start();

    // elog_set_filter();
}

/***
 * 函数名称 : Sys_Config();
 *
 * 函数描述 : 系统初始化配置;
 *
 * 传递值	: 无;
 *
 * 返回值   : 无;
 *
 **/
void Sys_Config(void)
{
    RCC_Configuration();

    GPIO_Configuration();

    NVIC_Configuration();

    USART_Configuration();

    // Delay_Configuration();

    SysTick_Configuration();

    /* third lib init */
    easy_logger_init();

    log_i("***********STM32 System Config!***********");
}

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
int __io_putchar(int ch)
{
    USART_SendData(USART1, (u8) ch);

    while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET));

    return ch;
}

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}
#else
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (u8) ch);

    while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET));

    return ch;
}

int fgetc(FILE *f)
{
    while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET));

    return (USART_ReceiveData(USART1));
}
#endif
