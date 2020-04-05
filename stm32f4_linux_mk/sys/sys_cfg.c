/**
  *****************************************************************************
  * @file    : sys_cfg.c
  * @author  : Tuu-图图
  * @version : 1.0.0
  * @date    : 2020-04-01
  * @brief   : sys config
  ******************************************************************************
  * @lasteditors  : Tuu-图图
  * @lasteditTime : 2020-04-01
  ******************************************************************************
  * @atten   : Copyright (C) by Tuu Inc
  *
  *****************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm_config.h"


/* Defines --------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/


/**
  * @note   RCC_Configuration
  * @brief  None
  * @param  None
  * @retval None
  */
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

/**
  * @note   GPIO_Configuration
  * @brief  None
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

    /* LED GPIO口配置 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_0);

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

/**
  * @note   NVIC_Configuration
  * @brief  None
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
}

/**
  * @note   USART_Configuration
  * @brief  None
  * @param  None
  * @retval None
  */
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

/**
  * @note   SysTick_Configuration
  * @brief  None
  * @param  None
  * @retval None
  */
static void SysTick_Configuration(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    uint32_t reload = configCPU_CLOCK_HZ / configTICK_RATE_HZ;

    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = reload;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/**
  * @note   easy_logger_init
  * @brief  None
  * @param  None
  * @retval None
  */
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

/**
  * @note   Sys_Config
  * @brief  None
  * @param  None
  * @retval None
  */
void Sys_Config(void)
{
    /* system init */
    RCC_Configuration();

    GPIO_Configuration();

    NVIC_Configuration();

    USART_Configuration();

    // SysTick_Configuration();

    /* user init */
    delay_init();

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

#if (defined configGENERATE_RUN_TIME_STATS) && (configGENERATE_RUN_TIME_STATS == 1)
#define USE_IRQ_TICK_CNT    0
#if USE_IRQ_TICK_CNT
static uint64_t rtos_run_time_cnt = 0;

/**
  * @note   TIM2_IRQHandler
  * @brief  None
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
        rtos_run_time_cnt++;
    }
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
#endif

/**
  * @note   rtos_sys_timer_init
  * @brief  None
  * @param  None
  * @retval None
  */
void rtos_sys_timer_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

#if !USE_IRQ_TICK_CNT
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
#else
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseStructure.TIM_Period = 100;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    rtos_run_time_cnt = 0;
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
#endif

    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @note   rtos_sys_cnt_get
  * @brief  None
  * @param  None
  * @retval None
  */
uint32_t rtos_sys_cnt_get(void)
{
#if USE_IRQ_TICK_CNT
    // return rtos_run_time_cnt;
#else
    return TIM_GetCounter(TIM2);
#endif
}
#endif

void delay_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);    //168_div_2 -> 84M clk

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);

    TIM_Cmd(TIM14, DISABLE);
}

/* sys delay code */
void delay_ms(uint32_t nTime)
{
    if (nTime == 0){
        return;
    }

    TIM_PrescalerConfig(TIM14, (84000/2-1), TIM_PSCReloadMode_Immediate);
    TIM_SetAutoreload(TIM14, nTime*2);
    TIM_ClearFlag(TIM14, TIM_FLAG_Update);

    TIM_Cmd(TIM14, ENABLE);

    while(TIM_GetFlagStatus(TIM14, TIM_FLAG_Update) == RESET);

    TIM_Cmd(TIM14, DISABLE);
}

void delay_us(uint32_t nTime)
{
    if (nTime == 0){
        return;
    }

    uint32_t time_ms;
    uint32_t time_us;

    time_ms = nTime/1000;
    delay_ms(time_ms);

    time_us = nTime%1000;

    TIM_PrescalerConfig(TIM14, (84-1), TIM_PSCReloadMode_Immediate);
    TIM_SetAutoreload(TIM14, time_us);
    TIM_ClearFlag(TIM14, TIM_FLAG_Update);

    TIM_Cmd(TIM14, ENABLE);

    while(TIM_GetFlagStatus(TIM14, TIM_FLAG_Update) == RESET);

    TIM_Cmd(TIM14, DISABLE);
}

void assert_failed(uint8_t* file, uint32_t line)
{
    os_printf("assert_failed:[%s][%d]", file, line);

    while(1){
    }
}
