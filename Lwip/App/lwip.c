/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */

/* USER CODE BEGIN 0 */
#include "lwip/udp.h"
#include "httpd.h"
#include "REST_api.h"
#include "persist_storage.h"
/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
/* ETH Variables initialization ----------------------------------------------*/
void _Error_Handler(char * file, int line);

/* USER CODE BEGIN 1 */
#define UDP_INPUT_PATTERN "BAMMBlBERkZGRjAgFw0xODEyMTExODAw"
#define UDP_RESP_PATTERN  "qY6iPkejkto+k/70MAoGCCqGSM49BAMC"
#define UDP_LISTEN_PORT   8082

/* This pcb is permanent, we should start listen UDP_LISTEN_PORT port
 * right after startup. On each UDP packet with UDP_INPUT_PATTERN payload
 * we should response using UDP packet with UDP_RESP_PATTERN payload */
struct udp_pcb *udp_listen_pcb_g = NULL;
/* Structure with network and device settings */
networkSettings_t networkSettings_g;
/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;

/* USER CODE BEGIN 2 */

void netif_status_callback(struct netif *netif)
{
  DBG_LOG(ETH_MGR, "Obtained IP address %u.%u.%u.%u", (uint8_t) (netif->ip_addr.addr),
          (uint8_t) (netif->ip_addr.addr >> 8), (uint8_t) (netif->ip_addr.addr >> 16),
          (uint8_t) (netif->ip_addr.addr >> 24));

  //ip_addr_debug_print_val(LWIP_DBG_ON, netif->ip_addr);
}

/* UDP Listen socket callback */
void udpCallbackFunction(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
  DBG_LOG(ETH_MGR, "Inside UDP callback");
  if (p != NULL)
  {
    if (0 == strncmp(p->payload, UDP_INPUT_PATTERN, strlen(UDP_INPUT_PATTERN)))
    {
      DBG_LOG(ETH_MGR, "Pattern OK. Reply");
      strncpy(p->payload, UDP_RESP_PATTERN, strlen(UDP_RESP_PATTERN));
      udp_sendto(pcb, p, addr, port);
    }
    pbuf_free(p);
  }
}

RV_t LW_GetNetworkSettingsPtr(networkSettings_t **ns)
{
	*ns = &networkSettings_g;

	return RV_SUCCESS;
}

static RV_t readNetworkSettings(void)
{
  uint8_t useDefaultSettings = 1;

  /* Read settings from persistent storage */
  if (RV_SUCCESS != PS_Read(NETWORK_CONFIG_SECTOR_START, (uint8_t *)(&networkSettings_g), sizeof(networkSettings_g)))
  {
	DBG_ERR(ETH_MGR, "Failed to read settings from flash memory");
	useDefaultSettings = 1;
  }

  if (VALIDATION_MARK != networkSettings_g.setValidationMark)
  {
	DBG_LOG(ETH_MGR, "There is no previously saved settings or settings has been corrupted");
	useDefaultSettings = 1;
  }

  if (useDefaultSettings || (0xFFFFFFFF == networkSettings_g.ipSet.ipaddr.addr))
  {
    /* IP addresses initialization */
	networkSettings_g.ipSet.ipaddr.addr  = ipaddr_addr(DEFAULT_IP_ADDRESS);
    networkSettings_g.ipSet.netmask.addr = ipaddr_addr(DEFAULT_NETMASK);
	networkSettings_g.ipSet.gw.addr      = ipaddr_addr(DEFAULT_GATEWAY);
	networkSettings_g.ipSet.dhcpEnable = 1;
	DBG_LOG(ETH_MGR, "Default network settings applied");
  }

  if (useDefaultSettings || ((0xFF == networkSettings_g.MACAddr[0]) && (0xFF == networkSettings_g.MACAddr[1])
				  && (0xFF == networkSettings_g.MACAddr[2]) && (0xFF == networkSettings_g.MACAddr[3])
				  && (0xFF == networkSettings_g.MACAddr[4]) && (0xFF == networkSettings_g.MACAddr[5])) ||
		  ((0x00 == networkSettings_g.MACAddr[0]) && (0x00 == networkSettings_g.MACAddr[1])
		  				  && (0x00 == networkSettings_g.MACAddr[2]) && (0x00 == networkSettings_g.MACAddr[3])
		  				  && (0x00 == networkSettings_g.MACAddr[4]) && (0x00 == networkSettings_g.MACAddr[5])))
  {
	uint8_t MACAddr[ETHARP_HWADDR_LEN] = DEFAULT_MAC_ADDRESS;

	/* Set MAC hardware address */
	memcpy(networkSettings_g.MACAddr, MACAddr, ETHARP_HWADDR_LEN);
	DBG_LOG(ETH_MGR, "Default MAC applied");
  }
  memcpy(gnetif.hwaddr, networkSettings_g.MACAddr, ETHARP_HWADDR_LEN);

  if (useDefaultSettings || (0x7FFFFFFF == (networkSettings_g.serialNumber & 0x7FFFFFFF)))
  {
    networkSettings_g.serialNumber = DEFAULT_SERIAL_NUM;
  }

  return RV_SUCCESS;
}

RV_t LW_SaveNetworkSettings(void)
{
  networkSettings_g.setValidationMark = VALIDATION_MARK;

  if (RV_SUCCESS != PS_Write(NETWORK_CONFIG_SECTOR_START, (uint8_t *)(&networkSettings_g), sizeof(networkSettings_g)))
  {
    DBG_ERR(ETH_MGR, "Failed to write settings to flash memory");
    return RV_FAILURE;
  }

  return RV_SUCCESS;
}

/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  err_t errorState = ERR_OK;

  readNetworkSettings();

  /* Initialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &networkSettings_g.ipSet.ipaddr, &networkSettings_g.ipSet.netmask,
		  &networkSettings_g.ipSet.gw, NULL, &ethernetif_init, &tcpip_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  gnetif.status_callback = netif_status_callback;

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

/* USER CODE BEGIN 3 */

  httpd_init();

  if (networkSettings_g.ipSet.dhcpEnable)
  {
	DBG_LOG(ETH_MGR, "DHCP setting is enabled. Start DHCP to obtain IP address");
	errorState = dhcp_start(&gnetif);
	if (ERR_OK != errorState)
	{
      DBG_ERR(ETH_MGR, "Failed to start DHCP. Error = %d", errorState);
	}
  }

  /* Start REST api */
  if (REST_API_OK != RM_InitRESTApi())
  {
	DBG_ERR(ETH_MGR, "Failed to start REST api");
  }

  /* Initialize UDP listen port */
  udp_listen_pcb_g = udp_new();

  errorState = udp_bind(udp_listen_pcb_g, IP_ADDR_ANY, UDP_LISTEN_PORT);
  if (ERR_OK != errorState)
  {
	DBG_ERR(ETH_MGR, "Failed to bind UDP socket. Error = %d", errorState);
  }

  /* Register UDP receive callback */
  udp_recv(udp_listen_pcb_g, udpCallbackFunction, NULL);

 /* USER CODE END 3 */
}

#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

/* USER CODE BEGIN 7 */
  sd = 0; // dummy code
/* USER CODE END 7 */
	
  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
/* USER CODE BEGIN 8 */
/* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */
  recved_bytes = 0; // dummy code
/* USER CODE END 9 */	
  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 10 */
  recved_bytes = 0; // dummy code
/* USER CODE END 10 */	
  return recved_bytes;
}
#endif /* MDK ARM Compiler */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
