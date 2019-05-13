/*
 * http_request.c
 *
 *  Created on: Oct 22, 2018
 *      Author: rspolyak
 *
 *      This file functionality handle incoming requests
 */

#include "http_request.h"
#include "pcs_json.h"


#define SWITCH_CONTINIOUS_DATA_URI       "/psw/switch/continuous/"         /* Continuous connection to transfer switch user actions */
#define SWITCH_ZONES_CONFIG_URI          "/psw/switch/zones/config"        /* Zones config uri */
#define SWITCH_SCENES_CONFIG_URI         "/psw/switch/scenes/config"       /* Scenes config uri */
#define SWITCH_SYSTEM_CONFIG_URI         "/psw/switch/system/config"       /* System config uri */
#define SWITCH_SCENE_STATUS_URI          "/psw/status/active-scene"        /* Active scene status uri */
#define SWITCH_ZONE_STATUS_URI           "/psw/status/zone-status"         /* Zone status uri */
#define SYSTEM_INFO_DATA_URI             "/psw/system/info"                /* System information uri */
#define DIAGNOSTICS_DATA_URI             "/psw/diagnostics"                /* Diagnostics error status uri */
#define IP_SETTINGS_URI                  "/psw/system/network"             /* Network settings uri */
#define SERIAL_NUMBER_URI                "/psw/system/serial-number"       /* Serial number uri */
#define MAC_ADDRESS_URI                  "/psw/system/mac-address"         /* MAC address uri */
#define STORED_APPLICATIONS_URI          "/psw/system/stored-applications" /* Stored applications uri */
#define REPROGRAMMING_URI                "/psw/system/code"                /* New firmware code uri */
#define REBOOT_URI                       "/psw/system/reboot"              /* Reboot command uri */
#define RESET_URI                        "/psw/system/reset"               /* Reset command uri */
#define CLOCK_SET_URI                    "/psw/system/clocktime"           /* Clock settings uri */

#define EXTERNAL_FLASH_URI               "/psw/system/external-flash"      /* Read and modify external flash memory */
#define INTERNAL_FLASH_URI               "/psw/system/internal-flash"      /* Read and modify internal flash memory */
#define INITIALIZE_FLASH_URI             "/psw/system/initialize"          /* Initialize external flash - ERASE all */
#define BOOT_ERROR_DETECT_URI            "/psw/system/boot-error"          /* Read and clear boot errors */

extern struct netif gnetif;

static char retURL[32];


/* NOTE: caller must ensure 'p' is valid pointer */
static rqst_t getPostRqstType(struct pbuf *p)
{
  if (0 == strncmp((char *) p->payload, "password", strlen("password")))
  {
    DBG_LOG(ETH_MGR, "Received 'password' request: %s", (char *) p->payload);

    return WEB_PAGE_LOGIN;
  }
  else if (0 == strncmp((char *) p->payload, "mode", strlen("mode")))
  {
    DBG_LOG(ETH_MGR, "Received 'mode' request");

    return WEB_PAGE_NETWORK;
  }

  return WEB_PAGE_UNKNOWN;
}

static RV_t getPasswordValue(struct pbuf *p, char *pass, uint8_t len)
{
  const char s[2] = "=";
  char *token = 0;

  if (!p || !pass)
  {
    DBG_ERR(ETH_MGR, "Null ptr received");
    return RV_NULLPTR;
  }

  token = strtok((char *) p->payload, s);

  if (token != NULL)
  {
    if (0 == strncmp(token, "password", sizeof("password")))
    {
      token = strtok(NULL, s);

      strncpy(pass, token, len);

      return RV_SUCCESS;
    }
  }

  return RV_FAILURE;
}

static RV_t getNetworkMode(struct pbuf *p, net_mode_t *mode, u32_t *ip, u32_t *mask, u32_t *gw)
{
  const char s[2] = "&";
  char *token = 0;

  if (!p || !mode || !ip || !mask || !gw)
  {
    DBG_ERR(ETH_MGR, "Null ptr received");
    return RV_NULLPTR;
  }

  token = strtok((char *) p->payload, s);

  while (token != NULL)
  {
    if (0 == strncmp(token, "mode", strlen("mode")))
    {
      if (0 == strncmp((token + sizeof("mode")), "dhcp", strlen("dhcp")))
      {
        *mode = MODE_DHCP;

        return RV_SUCCESS;
      }
      else
      {
        *mode = MODE_CUSTOM;
      }
    }
    else if (0 == strncmp(token, "ip", strlen("ip")))
    {
      *ip = ipaddr_addr(token + sizeof("ip"));
    }
    else if (0 == strncmp(token, "networkMask", strlen("networkMask")))
    {
      *mask = ipaddr_addr(token + sizeof("networkMask"));
    }
    else if (0 == strncmp(token, "gateway", strlen("gateway")))
    {
      *gw = ipaddr_addr(token + sizeof("gateway"));
    }
    else
    {
      return RV_FAILURE;
    }

    token = strtok(NULL, s);
  }

  return RV_SUCCESS;
}

/** Called when a POST request has been received. The application can decide
 * whether to accept it or not.
 *
 * @param connection Unique connection identifier, valid until httpd_post_end
 *        is called.
 * @param uri The HTTP header URI receiving the POST request.
 * @param http_request The raw HTTP request (the first packet, normally).
 * @param http_request_len Size of 'http_request'.
 * @param content_len Content-Length from HTTP header.
 * @param response_uri Filename of response file, to be filled when denying the
 *        request
 * @param response_uri_len Size of the 'response_uri' buffer.
 * @param post_auto_wnd Set this to 0 to let the callback code handle window
 *        updates by calling 'httpd_post_data_recved' (to throttle rx speed)
 *        default is 1 (httpd handles window updates automatically)
 * @return ERR_OK: Accept the POST request, data may be passed in
 *         another err_t: Deny the POST request, send back 'bad request'.
 */
err_t httpd_post_begin(void *connection, const char *uri, const char *http_request,
                       u16_t http_request_len, int content_len, char *response_uri,
                       u16_t response_uri_len, u8_t *post_auto_wnd)
{
  /* access all POST requests */

  memset(retURL, 0x00, sizeof(retURL));

  return ERR_OK;
}

/** Called when a POST request has been received. The application can decide
 * whether to accept it or not.
 *
 * @param connection Unique connection identifier, valid until httpd_post_end
 *        is called.
 * @param uri The HTTP header URI receiving the POST request.
 * @param http_request The raw HTTP request (the first packet, normally).
 * @param http_request_len Size of 'http_request'.
 * @param content_len Content-Length from HTTP header.
 * @param response_uri Filename of response file, to be filled when denying the
 *        request
 * @param response_uri_len Size of the 'response_uri' buffer.
 * @param post_auto_wnd Set this to 0 to let the callback code handle window
 *        updates by calling 'httpd_post_data_recved' (to throttle rx speed)
 *        default is 1 (httpd handles window updates automatically)
 * @return ERR_OK: Accept the POST request, data may be passed in
 *         another err_t: Deny the POST request, send back 'bad request'.
 */
err_t httpd_put_begin(void *connection, const char *uri, const char *http_request,
        u16_t http_request_len, int content_len, char *response_uri,
        u16_t response_uri_len, u8_t *post_auto_wnd)
{
	memset(retURL, 0x00, sizeof(retURL));
	return ERR_OK;
}

/* Custom function for handling custom GET requests
 *
 * @param pcb The tcp_pcb for which data is read
 * @param uri Pointer to the URL string
 * @param cusRet Flag for return custom data.
 * @note  need to return ERR_OK_CUS_RET when sending custom data
 * */

err_t httpd_get_receive_data(struct http_state *hs, char *uri, uint8_t *cusRet)
{
  char *respJson = NULL;
  RV_t status = RV_SUCCESS;

  if (uri == NULL)
  {
    DBG_ERR(ETH_MGR, "URL is empty");
    return ERR_ARG;
  }

  /* Check if we have received some request with json from server */
  if (0 == strncmp(uri, SWITCH_CONTINIOUS_DATA_URI, strlen(SWITCH_CONTINIOUS_DATA_URI)))
  {
    if (REST_API_OK != RM_SetupSwitchDataCommChannel(hs->pcb))
    {
      DBG_ERR(ETH_MGR, "Failed to create persistent switch data connection");
      status = RV_FAILURE;
    }
    else
    {
      /* Early return */
      *cusRet = ERR_OK_CUS_RET;
      return ERR_OK;
    }
  }
  else if (0 == strncmp(uri, IP_SETTINGS_URI, strlen(IP_SETTINGS_URI)))
  {
    if (REST_API_OK != RM_GenerateIPSettingsJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate IP settings json");
      status = RV_FAILURE;
    }
    else
    {
      DBG_LOG(ETH_MGR, "Received 'ip settings' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, SWITCH_ZONES_CONFIG_URI, strlen(SWITCH_ZONES_CONFIG_URI)))
  {
    if (REST_API_OK != RM_GenerateZoneConfigJson(&respJson))
    {
	  DBG_ERR(ETH_MGR, "Failed to generate zones config json");
	  status = RV_FAILURE;
	}
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'zones config' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, SWITCH_SCENES_CONFIG_URI, strlen(SWITCH_SCENES_CONFIG_URI)))
  {
    if (REST_API_OK != RM_GenerateSceneConfigJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate scenes config json");
	  status = RV_FAILURE;
    }
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'scenes config' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, SWITCH_SYSTEM_CONFIG_URI, strlen(SWITCH_SYSTEM_CONFIG_URI)))
  {
    if (REST_API_OK != RM_GenerateSystemConfigJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate system config json");
	  status = RV_FAILURE;
    }
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'system config' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, SYSTEM_INFO_DATA_URI, strlen(SYSTEM_INFO_DATA_URI)))
  {
    if (REST_API_OK != RM_GenerateInfoJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate info data json");
	  status = RV_FAILURE;
    }
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'info' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, DIAGNOSTICS_DATA_URI, strlen(DIAGNOSTICS_DATA_URI)))
  {
    if (REST_API_OK != RM_GenerateErrorStatusJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate error status json data");
	  status = RV_FAILURE;
    }
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'diagnostics' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, SERIAL_NUMBER_URI, strlen(SERIAL_NUMBER_URI)))
  {
    if (REST_API_OK != RM_GenerateSerialNumberJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate serial number json data");
	  status = RV_FAILURE;
    }
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'serial number' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, MAC_ADDRESS_URI, strlen(MAC_ADDRESS_URI)))
  {
    if (REST_API_OK != RM_GenerateMacAddressJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate mac address json data");
	  status = RV_FAILURE;
    }
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'mac address' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, STORED_APPLICATIONS_URI, strlen(STORED_APPLICATIONS_URI)))
  {
    if (REST_API_OK != RM_GenerateStoredApplicationsJson(&respJson))
    {
      DBG_ERR(ETH_MGR, "Failed to generate stored applications json data");
	  status = RV_FAILURE;
    }
    else
    {
	  DBG_LOG(ETH_MGR, "Received 'stored apps' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, BOOT_ERROR_DETECT_URI, strlen(BOOT_ERROR_DETECT_URI)))
  {
    if (REST_API_OK != RM_GenerateBootErrorJson(&respJson))
	{
	  DBG_ERR(ETH_MGR, "Failed to generate boot error detect json data");
	  status = RV_FAILURE;
	}
    else
    {
      DBG_LOG(ETH_MGR, "Received 'boot error detect' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, EXTERNAL_FLASH_URI, strlen(EXTERNAL_FLASH_URI)))
  {
	/* Retrieve address */
	uint32_t memAddr = (uint32_t) strtol(uri + strlen(EXTERNAL_FLASH_URI) + 1, NULL, 16);

	DBG_LOG(ETH_MGR, "Reading address = 0x%08lx", memAddr);

    if (REST_API_OK != RM_GenerateMemoryValueJson(&respJson, memAddr, 0))
	{
	  DBG_ERR(ETH_MGR, "Failed to generate external memory json data");
	  status = RV_FAILURE;
	}
    else
    {
      DBG_LOG(ETH_MGR, "Received 'read external memory' request. Give response:\r\n%s", respJson);
    }
  }
  else if (0 == strncmp(uri, INTERNAL_FLASH_URI, strlen(INTERNAL_FLASH_URI)))
  {
	/* Retrieve address */
	uint32_t memAddr = (uint32_t) strtol(uri + strlen(INTERNAL_FLASH_URI) + 1, NULL, 16);

	DBG_LOG(ETH_MGR, "Reading address = 0x%08lx", memAddr);

    if (REST_API_OK != RM_GenerateMemoryValueJson(&respJson, memAddr, 1))
	{
	  DBG_ERR(ETH_MGR, "Failed to generate internal memory json data");
	  status = RV_FAILURE;
	}
    else
    {
      DBG_LOG(ETH_MGR, "Received 'read internal memory' request. Give response:\r\n%s", respJson);
    }
  }

  else
  {
	/* Web server request. All is OK */
	return ERR_OK;
  }

  *cusRet = ERR_OK_CUS_RET;
  /* Disable keep alive */
  hs->keepalive = 0;

  if (RV_SUCCESS != status)
  {
    tcp_write(hs->pcb, HTTP_BAD_RESP, strlen(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
    tcp_output(hs->pcb);

    //return ERR_ABRT;
  }
  else
  {
    tcp_write(hs->pcb, HTTP_OK_RESP, strlen(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE);
	tcp_write(hs->pcb, respJson, strlen(respJson) + 1, TCP_WRITE_FLAG_COPY);
	tcp_output(hs->pcb);
  }

#ifdef HTTPD_TEST_CUS_PAGE
	if (!strcmp(CUS_URL_TEST_OK, uri))
	{
		tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
		*cusRet = ERR_OK_CUS_RET;
	}
	else if (!strcmp(CUS_URL_TEST_ERR, uri))
	{
		tcp_write(hs->pcb, HTTP_ERR_RESP, sizeof(HTTP_ERR_RESP), TCP_WRITE_FLAG_COPY);
		*cusRet = ERR_OK_CUS_RET;
	}
#endif

	/* TODO: pbuf_free(p); ? */

	return ERR_OK;
}

/** Called for each pbuf of data that has been received for a POST.
 * ATTENTION: The application is responsible for freeing the pbufs passed in!
 *
 * @param connection Unique connection identifier.
 * @param p Received data.
 * @param uri URL of the request.
 * @param cusRet Flag for return custom data.
 * @return ERR_OK: Data accepted.
 *         another err_t: Data denied, http_post_get_response_uri will be called.
 * @note  need to return ERR_OK_CUS_RET when sending custom data
 */
err_t httpd_post_receive_data(struct http_state *hs, struct pbuf *p, char *uri, uint8_t *cusRet)
{
  rqst_t rqstType;

  if (!p)
  {
    DBG_ERR(ETH_MGR, "Null ptr received");
    return ERR_ARG;
  }

  /* chained pbufs are not supported yet */
  if (p->tot_len > p->len)
  {
    DBG_ERR(ETH_MGR, "Chained pbuf received. Total len %u", p->tot_len);

    return ERR_VAL;
  }

  if (0 == strncmp(uri, REPROGRAMMING_URI, strlen(REPROGRAMMING_URI)))
  {
    DBG_LOG(ETH_MGR, "Received firmware update request");

    if (REST_API_OK != RM_SetupFirmwareDownloadCommChannel(hs->pcb, p->payload))
    {
      DBG_ERR(ETH_MGR, "Firmware update failed");

      /* Disable keep alive */
      hs->keepalive = 0;

      tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
      tcp_output(hs->pcb);
    }
    else
    {
      /* Protect from closing, make non blocking */
      hs->keepalive = 1;

      DBG_LOG(ETH_MGR, "Firmware update started");
    }

    *cusRet = ERR_OK_CUS_RET;

    pbuf_free(p);

    return ERR_OK;
  }
  else if (0 == strncmp(uri, REBOOT_URI, strlen(REBOOT_URI)))
  {
    DBG_LOG(ETH_MGR, "Received reboot request");

    /* Disable keep alive */
    hs->keepalive = 0;

    if (REST_API_OK != RM_ParceRebootJsonAndProcess(hs->pcb, p->payload))
    {
	  DBG_ERR(ETH_MGR, "Failed to parse reboot json");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	  tcp_output(hs->pcb);
	}

    *cusRet = ERR_OK_CUS_RET;

    pbuf_free(p);

    return ERR_OK;
  }
  else if (0 == strncmp(uri, RESET_URI, strlen(RESET_URI)))
  {
	DBG_LOG(ETH_MGR, "Received reset request");

    /* Disable keep alive */
    hs->keepalive = 0;

	if (REST_API_OK != RM_ResetProcess(hs->pcb))
	{
	  DBG_ERR(ETH_MGR, "Failed to reset mcu");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	  tcp_output(hs->pcb);
	}

    *cusRet = ERR_OK_CUS_RET;

    pbuf_free(p);

    return ERR_OK;
  }
  else if (0 == strncmp(uri, INITIALIZE_FLASH_URI, strlen(INITIALIZE_FLASH_URI)))
  {
    DBG_LOG(ETH_MGR, "Received external flash initialize request");

    /* Disable keep alive */
    hs->keepalive = 0;

    if (REST_API_OK != RM_ExtFlashInitProcess())
    {
	  DBG_ERR(ETH_MGR, "Failed to erase entire external flash");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	  tcp_output(hs->pcb);
	}
    else
    {
      DBG_LOG(ETH_MGR, "External flash would be erased");

      tcp_write(hs->pcb, HTTP_OK_RESP, strlen(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
      tcp_output(hs->pcb);
    }

    *cusRet = ERR_OK_CUS_RET;

    pbuf_free(p);

    return ERR_OK;
  }

  rqstType = getPostRqstType(p);

    switch (rqstType)
    {
    case WEB_PAGE_LOGIN:
    {
      char pass[16] = {0};
      char *redundantSymbol = 0;

      if (RV_SUCCESS == getPasswordValue(p, pass, sizeof(pass)))
      {
        /* TODO: investigate why password is appended with '\r\n'.
         * For now, just strip redundant symbols */
        redundantSymbol = strchr(pass, '\r');
        if (redundantSymbol)
        {
          *redundantSymbol = '\0';
        }

        redundantSymbol = strchr(pass, '\n');
        if (redundantSymbol)
        {
          *redundantSymbol = '\0';
        }

        /* if valid password with correct length was entered */
        if ((!strncmp(pass, SAGE_PASSWORD, strlen(SAGE_PASSWORD))) && (strlen(pass) == strlen(SAGE_PASSWORD)))
        {
          strncpy(retURL, "/main.shtml", sizeof(retURL));
        }
        else
        {
          strncpy(retURL, "/404.html", sizeof(retURL));
        }
      }
    }
    break;

      case WEB_PAGE_NETWORK:
      {
        net_mode_t mode = MODE_CUSTOM;
        ip4_addr_t ipaddr;
        ip4_addr_t netmask;
        ip4_addr_t gw;

        ipaddr.addr = 0;
        netmask.addr = 0;
        gw.addr = 0;

        networkSettings_t *settings;

        if (RV_SUCCESS == getNetworkMode(p, &mode, &ipaddr.addr, &netmask.addr, &gw.addr))
        {
          LW_GetNetworkSettingsPtr(&settings);

          if (MODE_CUSTOM == mode)
          {
        	settings->ipSet.ipaddr.addr = ipaddr.addr;
        	settings->ipSet.netmask.addr = netmask.addr;
        	settings->ipSet.gw.addr = gw.addr;
        	settings->ipSet.dhcpEnable = 0;
            //netif_set_addr(&gnetif, &ipaddr, &netmask, &gw);
          }
          else
          {
        	settings->ipSet.dhcpEnable = 1;
            //dhcp_start(&gnetif);
          }

          if (RV_SUCCESS != LW_SaveNetworkSettings())
          {
        	DBG_ERR(ETH_MGR, "Failed to save network settings received from the web");
          }

          strncpy(retURL, "/main.shtml", sizeof(retURL));
        }
        else
        {
          strncpy(retURL, "/404.html", sizeof(retURL));
        }
      }
      break;

      case WEB_PAGE_UNKNOWN:
      default:
      {
        strncpy(retURL, "/404.html", sizeof(retURL));
      }
      break;
    }

  pbuf_free(p);

  return ERR_OK;
}

/** Called for each pbuf of data that has been received for a POST.
 * ATTENTION: The application is responsible for freeing the pbufs passed in!
 *
 * @param connection Unique connection identifier.
 * @param p Received data.
 * @param uri URL of the request.
 * @param cusRet Flag for return custom data.
 * @return ERR_OK: Data accepted.
 *         another err_t: Data denied, http_post_get_response_uri will be called.
 * @note  need to return ERR_OK_CUS_RET when sending custom data
 */
err_t httpd_put_receive_data(struct http_state *hs, struct pbuf *p, char *uri, uint8_t *cusRet)
{
  if (!p)
  {
    DBG_ERR(ETH_MGR, "Null ptr received");
    return ERR_ARG;
  }

  if (0 == strncmp(uri, IP_SETTINGS_URI, strlen(IP_SETTINGS_URI)))
  {
    DBG_LOG(ETH_MGR, "Received IP settings put request");

    /* Disable keep alive */
    hs->keepalive = 0;

    if (REST_API_OK != RM_ParseIPSettingsJsonAndProcess(p->payload))
    {
      DBG_ERR(ETH_MGR, "Failed to parse ip settings json or do required set-up");

      tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
    }
    else
    {
      DBG_LOG(ETH_MGR, "New ip settings will be applied soon");

      tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
    }
    tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, SERIAL_NUMBER_URI, strlen(SERIAL_NUMBER_URI)))
  {
    DBG_LOG(ETH_MGR, "Received serial number put request");

    /* Disable keep alive */
    hs->keepalive = 0;

    if (REST_API_OK != RM_ParseSerialNumberJsonAndProcess(p->payload))
    {
      DBG_ERR(ETH_MGR, "Failed to parse serial number json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
	  DBG_LOG(ETH_MGR, "Successfully set new serial number");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
    tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, MAC_ADDRESS_URI, strlen(MAC_ADDRESS_URI)))
  {
    DBG_LOG(ETH_MGR, "Received mac address put request");

    /* Disable keep alive */
    hs->keepalive = 0;

    if (REST_API_OK != RM_ParseMacAddressJsonAndProcess(p->payload))
    {
      DBG_ERR(ETH_MGR, "Failed to parse mac address json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
	  DBG_LOG(ETH_MGR, "New MAC address will be applied soon");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
    tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, BOOT_ERROR_DETECT_URI, strlen(BOOT_ERROR_DETECT_URI)))
  {
    DBG_LOG(ETH_MGR, "Received error status put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseBootErrorJsonAndProcess(p->payload))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse error status json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "New error status has been successfully set");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, EXTERNAL_FLASH_URI, strlen(EXTERNAL_FLASH_URI)))
  {
    DBG_LOG(ETH_MGR, "Received external memory put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseMemoryChangeJsonAndProcess(p->payload, 0))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse memory change json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "External memory has been successfully set");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, INTERNAL_FLASH_URI, strlen(INTERNAL_FLASH_URI)))
  {
    DBG_LOG(ETH_MGR, "Received internal memory put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseMemoryChangeJsonAndProcess(p->payload, 1))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse memory change json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "Internal memory has been successfully set");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, CLOCK_SET_URI, strlen(CLOCK_SET_URI)))
  {
    DBG_LOG(ETH_MGR, "Received clock settings put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseClockJsonAndProcess(p->payload))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse clock json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "Clock settings have been applied");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, SWITCH_ZONES_CONFIG_URI, strlen(SWITCH_ZONES_CONFIG_URI)))
  {
    DBG_LOG(ETH_MGR, "Received zone settings put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseZoneConfigJsonAndProcess(p->payload))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse zone json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "Zones settings have been applied");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, SWITCH_SCENES_CONFIG_URI, strlen(SWITCH_SCENES_CONFIG_URI)))
  {
    DBG_LOG(ETH_MGR, "Received scene settings put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseSceneConfigJsonAndProcess(p->payload))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse scene json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "Scenes settings have been applied");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, SWITCH_SYSTEM_CONFIG_URI, strlen(SWITCH_SYSTEM_CONFIG_URI)))
  {
    DBG_LOG(ETH_MGR, "Received system settings put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseSystemConfigJsonAndProcess(p->payload))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse system json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "System settings have been applied");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, SWITCH_SCENE_STATUS_URI, strlen(SWITCH_SCENE_STATUS_URI)))
  {
    DBG_LOG(ETH_MGR, "Received scene status put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseSceneStatusJsonAndProcess(p->payload))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse scene status json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "Scene status has been applied");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }
  else if (0 == strncmp(uri, SWITCH_ZONE_STATUS_URI, strlen(SWITCH_ZONE_STATUS_URI)))
  {
    DBG_LOG(ETH_MGR, "Received zone status put request");

	/* Disable keep alive */
	hs->keepalive = 0;

	if (REST_API_OK != RM_ParseZoneStatusJsonAndProcess(p->payload))
	{
	  DBG_ERR(ETH_MGR, "Failed to parse zone status json or do required set-up");

	  tcp_write(hs->pcb, HTTP_BAD_RESP, sizeof(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
	}
	else
	{
      DBG_LOG(ETH_MGR, "Zone status has been applied");

	  tcp_write(hs->pcb, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
	}
	tcp_output(hs->pcb);
  }

  *cusRet = ERR_OK_CUS_RET;

#ifdef HTTPD_TEST_CUS_PAGE
	if (!strcmp(uri, "/in"))
	{
		/* Return index page (for test only) */
		strncpy(retURL, "/index.shtml", sizeof(retURL));
	}
	else if (!strcmp(uri, "/out"))
	{
		tcp_write(hs->pcb, "\r\n{\r\n\t\"fake data\": 13\r\n}\r\n", sizeof("\r\n{\r\n\t\"fake data\": 13\r\n}\r\n"), TCP_WRITE_FLAG_COPY);
		*cusRet = ERR_OK_CUS_RET;
	}
#endif

  pbuf_free(p);

  return ERR_OK;
}

/** Called when all data is received or when the connection is closed.
 * The application must return the filename/URI of a file to send in response
 * to this POST request. If the response_uri buffer is untouched, a 404
 * response is returned.
 *
 * @param connection Unique connection identifier.
 * @param response_uri Filename of response file, to be filled when denying the request
 * @param response_uri_len Size of the 'response_uri' buffer.
 */
void httpd_request_finished(void *connection, char *response_uri, u16_t response_uri_len)
{
  strncpy(response_uri, retURL, response_uri_len);
}


