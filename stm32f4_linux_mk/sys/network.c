/**
  ******************************************************************************
  * @file    stm32f4x7_eth_bsp.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   STM32F4x7 Ethernet hardware configuration.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
#include "lwip/netif.h"


/* Defines --------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void ETH_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                            RCC_AHB1Periph_GPIOB |
                            RCC_AHB1Periph_GPIOC,
                            ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#ifdef MII_MODE
#ifdef PHY_CLOCK_MCO
    RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
#endif /* PHY_CLOCK_MCO */
    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);
#elif defined RMII_MODE
    RCC_MCO1Config(RCC_MCO1Source_PLLCLK,RCC_MCO1Div_2);
    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
#endif

/* Ethernet pins configuration ************************************************/
    /*
        ETH_MDIO -------------------------> PA2
        ETH_MDC --------------------------> PC1
        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PB11
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PB12
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PB13
        ETH_MII_RESET/ETH_RMII_RESET -----> PC0
                                                  */

    /* Configure PA1, PA2 and PA7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

    /* Configure PB11,PB12 and PB13 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH);

    /* Configure PC1, PC4 and PC5 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static uint32_t ETH_MACDMA_Config(void)
{
    uint32_t EthStatus = 0;
    ETH_InitTypeDef ETH_InitStructure;

    /* Enable ETHERNET clock  */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                            RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    while (ETH_GetSoftwareResetStatus() == SET);

    /* ETHERNET Configuration --------------------------------------------------*/
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);

    /* Fill ETH_InitStructure parametrs */
    /*------------------------   MAC   -----------------------------------*/
#if AUTONEGOTIATION
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
#else
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
    ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
#endif

    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

    /*------------------------   DMA   -----------------------------------*/

    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
    the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
    if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    /* Configure Ethernet */
    EthStatus = ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

    return EthStatus;
}

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
uint32_t ETH_BSP_Config(void)
{
    uint32_t EthStatus = 0;
    RCC_ClocksTypeDef RCC_Clocks;

    /***************************************************************************
        NOTE:
            When using Systick to manage the delay in Ethernet driver, the Systick
            must be configured before Ethernet initialization and, the interrupt
            priority should be the highest one.
    *****************************************************************************/

    /* Configure Systick clock source as HCLK */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    /* SystTick configuration: an interrupt every 10ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

    /* Set Systick interrupt priority to 0*/
    NVIC_SetPriority (SysTick_IRQn, 0);

    /* Configure the GPIO ports for ethernet pins */
    ETH_GPIO_Config();

    /* Configure the Ethernet MAC/DMA */
    ETH_MACDMA_Config();

    /* Get Ethernet link status*/
    if(ETH_ReadPHYRegister(PHY_ADDRESS, PHY_SR) & PHY_DUPLEX_STATUS)
    {
        EthStatus |= ETH_LINK_FLAG;
    }

    return EthStatus;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
