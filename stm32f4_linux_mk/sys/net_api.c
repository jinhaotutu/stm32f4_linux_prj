/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Network connection configuration
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
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/inet.h"
#include "Standalone/ethernetif.h"

#include "stm_config.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    DHCP_START,
    DHCP_WAIT_ADDRESS,
    DHCP_ADDRESS_ASSIGNED,
    DHCP_TIMEOUT,
    DHCP_LINK_DOWN,
}DHCP_STA;


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static struct netif gnetif;

static uint32_t sys_time = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function handles Ethernet link status.
  * @param  None
  * @retval None
  */
void link_status_change(uint8_t status)
{
  /* Check whether the link interrupt has occurred or not */
    if(status == true)
    {
        LED2_ON;
        netif_set_link_up(&gnetif);
    }
    else
    {
        LED2_OFF;
        netif_set_link_down(&gnetif);
    }
}

/**
  * @brief  Link callback function, this function is called on change of link status.
  * @param  The network interface
  * @retval None
  */
void ETH_link_callback(struct netif *netif)
{
    __IO uint32_t timeout = 0;
    uint32_t tmpreg,RegValue;
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    if(netif_is_link_up(netif))
    {
        /* When the netif is fully configured this function must be called.*/
        netif_set_up(&gnetif);
        log_d("netif_set_up");
    }
    else
    {
        /*  When the netif link is down this function must be called.*/
        netif_set_down(&gnetif);
        log_d("netif_set_down");
    }
}

/**
  * @brief  dhcp
  * @param  None
  * @retval None
  */
static void LwIP_DHCP_task(void *p)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    uint32_t IPaddress;

    DHCP_STA dhcp_status = DHCP_START;

    while(1)
    {
        switch (dhcp_status)
        {
            case DHCP_START:
                dhcp_start(&gnetif);
                /* IP address should be set to 0
                every time we want to assign a new DHCP address */
                IPaddress = 0;
                dhcp_status = DHCP_WAIT_ADDRESS;

                break;

            case DHCP_WAIT_ADDRESS:
                /* Read the new IP address */
                IPaddress = gnetif.ip_addr.addr;

                if (IPaddress!=0)
                {
                    dhcp_status = DHCP_ADDRESS_ASSIGNED;

                    /* Stop DHCP */
                    dhcp_stop(&gnetif);

                    log_i("use dhcp ip");
                    log_i("got ipadder: %s", inet_ntoa(gnetif.ip_addr.addr));
                    log_i("got netmask: %s", inet_ntoa(gnetif.netmask.addr));
                    log_i("got gwadder: %s", inet_ntoa(gnetif.gw.addr));

                    vTaskDelete(NULL);
                }
                else
                {
                    /* DHCP timeout */
                    if (gnetif.dhcp->tries > 4)
                    {
                        dhcp_status = DHCP_TIMEOUT;

                        /* Stop DHCP */
                        dhcp_stop(&gnetif);

                        /* Static address used */
                        IP4_ADDR(&ipaddr, 192, 168, 1, 88);
                        IP4_ADDR(&netmask, 255, 255, 255, 0);
                        IP4_ADDR(&gw, 192, 168, 1, 1);
                        netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);

                        log_i("use static ip");
                        log_i("got ipadder: %s", inet_ntoa(gnetif.ip_addr.addr));
                        log_i("got netmask: %s", inet_ntoa(gnetif.netmask.addr));
                        log_i("got gwadder: %s", inet_ntoa(gnetif.gw.addr));

                        vTaskDelete(NULL);
                    }
                }

                break;

            default:
                break;
        }

        /* wait 250 ms */
        vTaskDelay(250);
    }
}

/**
* @brief  Initializes the lwIP stack
* @param  None
* @retval None
*/
uint32_t NetWork_Init(void)
{
    uint32_t EthStatus = 0;

    struct ip_addr ipaddr={0};
    struct ip_addr netmask={0};
    struct ip_addr gw={0};

    /* eth init */
    EthStatus = ETH_BSP_Config();
    log_d("EthStatus:0x%02x", EthStatus);

    tcpip_init(NULL, NULL);

    /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
    struct ip_addr *netmask, struct ip_addr *gw,
    void *state, err_t (* init)(struct netif *netif),
    err_t (* input)(struct pbuf *p, struct netif *netif))

    Adds your network interface to the netif_list. Allocate a struct
    netif and pass a pointer to this structure as the first argument.
    Give pointers to cleared ip_addr structures when using DHCP,
    or fill them with sane numbers otherwise. The state pointer may be NULL.

    The init function pointer must point to a initialization function for
    your ethernet netif interface. The following code illustrates it's use.*/
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);

    /*  Registers the default network interface.*/
    netif_set_default(&gnetif);

    if (EthStatus == (ETH_INIT_FLAG | ETH_LINK_FLAG))
    {
        log_d("netif_set_up");
        /* Set Ethernet link flag */
        gnetif.flags |= NETIF_FLAG_LINK_UP;

        /* When the netif is fully configured this function must be called.*/
        netif_set_up(&gnetif);

        sys_thread_new("dhcp", LwIP_DHCP_task, NULL, 1024, (configMAX_PRIORITIES - 2));
    }
    else
    {
        log_d("netif_set_down");
        /*  When the netif link is down this function must be called.*/
        netif_set_down(&gnetif);
    }

    /* Set the link callback function, this function is called on change of link status*/
    netif_set_link_callback(&gnetif, ETH_link_callback);

    return 1;
}

/**
* @brief  Initializes the lwIP stack
* @param  None
* @retval None
*/
uint32_t NetWork_Init_no_os(void)
{
    uint32_t EthStatus = 0;

    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    /* eth init */
    EthStatus = ETH_BSP_Config();
    log_d("EthStatus:0x%02x", EthStatus);

    lwip_init();

    IP4_ADDR(&ipaddr, 192, 168, 1, 88);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw, 192, 168, 1, 1);

    /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
    struct ip_addr *netmask, struct ip_addr *gw,
    void *state, err_t (* init)(struct netif *netif),
    err_t (* input)(struct pbuf *p, struct netif *netif))

    Adds your network interface to the netif_list. Allocate a struct
    netif and pass a pointer to this structure as the first argument.
    Give pointers to cleared ip_addr structures when using DHCP,
    or fill them with sane numbers otherwise. The state pointer may be NULL.

    The init function pointer must point to a initialization function for
    your ethernet netif interface. The following code illustrates it's use.*/
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);

    /*  Registers the default network interface.*/
    netif_set_default(&gnetif);

    if (EthStatus == (ETH_INIT_FLAG | ETH_LINK_FLAG))
    {
        log_d("netif_set_up");
        /* Set Ethernet link flag */
        gnetif.flags |= NETIF_FLAG_LINK_UP;

        /* When the netif is fully configured this function must be called.*/
        netif_set_up(&gnetif);
    }
    else
    {
        log_d("netif_set_down");
        /*  When the netif link is down this function must be called.*/
        netif_set_down(&gnetif);
    }

    /* Set the link callback function, this function is called on change of link status*/
    netif_set_link_callback(&gnetif, ETH_link_callback);
    return 1;
}

void update_lwip_tick(uint32_t tick)
{
    sys_time += tick;
}

/**
* @brief  Called when a frame is received
* @param  None
* @retval None
*/
void LwIP_Pkt_Handle(void)
{
    /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
    while(ETH_CheckFrameReceived()){
        ethernetif_input(&gnetif);
    }
}

void LwIP_IRQ_Post(void)
{
    ethernetif_post(&gnetif);
}

/**
* @brief  LwIP periodic tasks
* @param  localtime the current LocalTime value
* @retval None
*/
void LwIP_Periodic_Handle(void)
{
    static uint32_t ARPTimer = 0;
    static uint32_t TCPTimer = 0;

#if LWIP_TCP
    /* TCP periodic process every 250 ms */
    if (sys_time - TCPTimer >= TCP_TMR_INTERVAL)
    {
        TCPTimer =  sys_time;
        tcp_tmr();
    }
#endif

    /* ARP periodic process every 5s */
    if ((sys_time - ARPTimer) >= ARP_TMR_INTERVAL)
    {
        ARPTimer =  sys_time;
        etharp_tmr();
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
