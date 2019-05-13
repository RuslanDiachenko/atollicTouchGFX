/*******************************************************************************
* @file  REST_api.c
* @brief Sage Glass REST api implementation
*
* This file provides functions to send and receive REST api requests
*
*******************************************************************************/

#include <string.h>
#include "pcs_json.h"

#include "REST_api.h"
#include "http_request.h"
#include "fw_update.h"
#include "switch_manager.h"

#define SWITCH_PING_SEC 300  /* Send ping message each 5 minutes */

#define REST_API_STACK_SIZE   512
#define NUM_OF_SUBSCRIBERS    8  /* Extend later if there will be such need */

/* Request length settings */
#if REST_OUT_RQST_ENABLE
#define REST_API_MAX_BUF_LEN  512
#define REST_API_MAX_MSG_LEN  314
#define MIN_PUT_POST_RQST_LEN 100
#define MIN_GET_RQST_LEN       50
#else
#define REST_API_MAX_BUF_LEN  FW_UPD_CHUNK_SIZE /* For firmware updates only */
#endif

#define REST_MGR "REST"

#if REST_OUT_RQST_ENABLE
#define CONTENT_TYPE_FIELD  "Content-Type: application/json"
#define CONTENT_LEGTH_FIELD "Content-Length: "
#define HOST_FIELD          "Host: "

#define POST_CMD_BASE "POST %s HTTP/1.1\r\n%s%s\r\n%s\r\n%s%d\r\n\r\n%s\r\n\r\n"
#define PUT_CMD_BASE  "PUT %s HTTP/1.1\r\n%s%s\r\n%s\r\n%s%d\r\n\r\n%s\r\n\r\n"
#define GET_CMD_BASE  "GET %s HTTP/1.1\r\n%s%s\r\n\r\n"

typedef struct
{
  restApiRqstType_t type;
  restApiResp_cb_t cb;
  uint32_t destIP;
  uint16_t port;
  char uri[REST_API_MAX_URI_LEN];
  uint16_t payloadLen;
  uint8_t *payload;
} restApiMsg_t;

enum client_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};

struct client_struct
{
  enum client_states state;     /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};
#endif

/* Delayed actions part:
 * Some remote commands should be delayed in time
 * to allow lwip stack send response to control unit */
#define DELAYED_ACTION_TIMEOUT_SEC 2

enum delayed_states
{
  DS_NONE,
  DS_IP_SET,
  DS_MAC_SET,
  DS_REBOOT,
  DS_RESET,
  DS_UPDATE
};

struct delayedAction_struct
{
  enum delayed_states state;
  networkSettings_t *settings;
};
struct delayedAction_struct das_g;

static osTimerId switchPingTimerHandle;
static osTimerId delayedActionTimerHandle;
static osTimerId fwUpdateTimerHandle;

struct tcp_pcb *switch_pcb_g = NULL;
struct tcp_pcb *fwUpd_pcb_g  = NULL;
struct http_state *fwUpd_http_g = NULL;
/* Firmware update control variables */
static uint16_t fwReceiveTimeout = 0;
static uint8_t waitingForFWChunk = 0;

extern struct netif gnetif;
extern ETH_HandleTypeDef heth;

#if REST_OUT_RQST_ENABLE
static osMailQDef(restMsgBox_g, 5, restApiMsg_t);
static osMailQId  restMsgBox_g;

struct tcp_pcb *client_pcb_g = NULL;
struct client_struct *cs_g;
static restApiMsg_t *apiContext_g = NULL;
#endif

char restApiMsgBuf_g[REST_API_MAX_BUF_LEN]; //<---- in case out of memory consider to use dynamical allocation
/* Used to handle several receive messages */
char *restApiMsgBufPtr_g = 0;

#if REST_OUT_RQST_ENABLE
static uint16_t connNum_g = 1;

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client_struct * es);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_client_send(struct tcp_pcb *tpcb, struct client_struct * es);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
static void tcp_client_err(void *arg, err_t err);

static restApiStatus_t sendMsgFromRESTApiQueue(restApiMsg_t *mail);
#endif


restApiStatus_t RM_GenerateInfoJson(char **json)
{
  fw_settings_t *fw_settings = NULL;
  char serial[10] = {0};

  FW_GetSettingsPtr(&fw_settings);
  snprintf(serial, sizeof(serial), "%08d", das_g.settings->serialNumber);

  JSON_Start();
  JSON_Add_Field_String("type", "switch");
  JSON_Add_Field_String("serialNumber", serial);
  JSON_Add_Field_Integer("appMajor", fw_settings->internalApp.version.vMajor);
  JSON_Add_Field_Integer("appMinor", fw_settings->internalApp.version.vMinor);
  JSON_Add_Field_Integer("appBuild", fw_settings->internalApp.version.vBuild);
  JSON_Add_Field_Integer("bootMajor", fw_settings->bootCodeVer.vMajor);
  JSON_Add_Field_Integer("bootMinor", fw_settings->bootCodeVer.vMinor);
  JSON_Add_Field_Integer("bootBuild", fw_settings->bootCodeVer.vBuild);
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_GenerateErrorStatusJson(char **json)
{
  uint8_t genErr = 0;
  uint8_t tbd1Failed = 0;
  uint8_t tbd2Failed = 0;

  if (RV_SUCCESS != SW_GetErrorStatus(&genErr, &tbd1Failed, &tbd2Failed))
  {
    DBG_ERR(REST_MGR, "Unable to get fail status data");
    return REST_API_FAIL;
  }

  JSON_Start();
  JSON_Add_Field_Integer("errorStatus", genErr);
  JSON_Add_Field_Integer("TBD1Failed", tbd1Failed);
  JSON_Add_Field_Integer("TBD2Failed", tbd2Failed);
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_GenerateIPSettingsJson(char **json)
{
  int dhcpEnable = 0;
  char addrString[REST_API_MAX_IP_LEN] = {0};

  if (NULL != ((struct dhcp*)netif_get_client_data(&gnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP)))
  {
    dhcpEnable = 1;
  }

  JSON_Start();
  JSON_Add_Field_Integer("dhcpClientEnabled", dhcpEnable);
  ip4addr_ntoa_r(&gnetif.ip_addr, addrString, sizeof(addrString));
  JSON_Add_Field_String("ipAddress", addrString);
  ip4addr_ntoa_r(&gnetif.netmask, addrString, sizeof(addrString));
  JSON_Add_Field_String("ipSubnetMask", addrString);
  ip4addr_ntoa_r(&gnetif.gw, addrString, sizeof(addrString));
  JSON_Add_Field_String("ipGateway", addrString);
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseIPSettingsJsonAndProcess(char *json)
{
  int dhcpEnable = 0;
  char addrString[REST_API_MAX_IP_LEN] = {0};

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  /* Individual network parameters may be set via this request */

  if (JSON_Extract_Integer(json, "dhcpClientEnabled", &dhcpEnable) && dhcpEnable)
  {
	DBG_LOG(REST_MGR, "DHCP client will be enabled at next startup");
  }

  if (dhcpEnable)
  {
    /* Ignore other fields, because we should use dhcp */
	das_g.settings->ipSet.dhcpEnable = 1;
  }
  else
  {
    if (JSON_Extract_String(json, "ipAddress", addrString, sizeof(addrString)))
    {
      das_g.settings->ipSet.ipaddr.addr = ipaddr_addr(addrString);
    }

    if (JSON_Extract_String(json, "ipSubnetMask", addrString, sizeof(addrString)))
    {
      das_g.settings->ipSet.netmask.addr = ipaddr_addr(addrString);
    }

    if (JSON_Extract_String(json, "ipGateway", addrString, sizeof(addrString)))
    {
      das_g.settings->ipSet.gw.addr = ipaddr_addr(addrString);
    }
    das_g.settings->ipSet.dhcpEnable = 0;
  }

  if (RV_SUCCESS != LW_SaveNetworkSettings())
  {
    DBG_ERR(REST_MGR, "Failed to save network settings to flash memory");
    return REST_API_FAIL;
  }

  /* Do required settings in 2 seconds to allow lwip stack to send response */
  //das_g.state = DS_IP_SET;
  //osTimerStart(delayedActionTimerHandle, pdMS_TO_TICKS(1000 * DELAYED_ACTION_TIMEOUT_SEC));

  return REST_API_OK;
}

restApiStatus_t RM_GenerateSerialNumberJson(char **json)
{
  char serial[10] = {0};

  snprintf(serial, sizeof(serial), "%08d", das_g.settings->serialNumber);

  JSON_Start();
  JSON_Add_Field_String("serialNumber", serial);
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseSerialNumberJsonAndProcess(char *json)
{
  char serial[10] = {0};

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  if (0 == JSON_Extract_String(json, "value", serial, sizeof(serial)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'value' field from received json");
    return REST_API_FAIL;
  }

  das_g.settings->serialNumber = atoi(serial);

  if (RV_SUCCESS != LW_SaveNetworkSettings())
  {
    DBG_ERR(REST_MGR, "Failed to save serial number setting to flash memory");
    return REST_API_FAIL;
  }

  return REST_API_OK;
}

restApiStatus_t RM_GenerateMacAddressJson(char **json)
{
  char macString[32] = {0};
  uint32_t highMAC = heth.Instance->MACA0HR;
  uint32_t lowMAC = heth.Instance->MACA0LR;

  snprintf(macString, sizeof(macString), "%02x:%02x:%02x:%02x:%02x:%02x",
		  (uint8_t)lowMAC, (uint8_t)(lowMAC >> 8), (uint8_t)(lowMAC >> 16),
		  (uint8_t)(lowMAC >> 24), (uint8_t)highMAC, (uint8_t)(highMAC >> 8));

  JSON_Start();
  JSON_Add_Field_String("macAddress", macString);
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseMacAddressJsonAndProcess(char *json)
{
  char macString[32] = {0};
  char *macString_p = NULL;
  uint8_t macAddr[ETHARP_HWADDR_LEN] = {0};

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  if (0 == JSON_Extract_String(json, "value", macString, sizeof(macString)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'value' field from received json");
    return REST_API_FAIL;
  }

  /* Retrieve MAC address from string */
  macString_p = macString;
  for (uint8_t i = 0; i < sizeof(macAddr); i++)
  {
	macAddr[i] = (uint8_t) strtol(macString_p, NULL, 16);
    if (i != (sizeof(macAddr) - 1))
    {
      macString_p = strchr(macString_p, ':') + 1;
    }
    if (NULL == macString_p)
    {
      DBG_ERR(REST_MGR, "Incorrect format of MAC address received");
      return REST_API_FAIL;
    }
  }

  memset(das_g.settings->MACAddr, 0x00, sizeof(das_g.settings->MACAddr));
  memcpy(das_g.settings->MACAddr, macAddr, sizeof(das_g.settings->MACAddr));

  DBG_LOG(REST_MGR, "New MAC address: %02x:%02x:%02x:%02x:%02x:%02x will be applied after reboot",
		  das_g.settings->MACAddr[0], das_g.settings->MACAddr[1], das_g.settings->MACAddr[2],
		  das_g.settings->MACAddr[3], das_g.settings->MACAddr[4], das_g.settings->MACAddr[5]);

  if (RV_SUCCESS != LW_SaveNetworkSettings())
  {
    DBG_ERR(REST_MGR, "Failed to save network settings to flash memory. Skip reboot");
    return REST_API_FAIL;
  }

  //das_g.state = DS_MAC_SET;
  //osTimerStart(delayedActionTimerHandle, pdMS_TO_TICKS(1000 * DELAYED_ACTION_TIMEOUT_SEC));

  return REST_API_OK;
}

restApiStatus_t RM_GenerateZoneConfigJson(char **json)
{
  char name[16] = {0};
  zone_config_t zone;
  uint8_t zonesNum = 0;

  if (RV_SUCCESS != SW_GetZoneNum(&zonesNum))
  {
    DBG_ERR(REST_MGR, "Unable to get number of zones");
    return REST_API_FAIL;
  }

  JSON_Start();
  JSON_Start_Array("Zones");
  for (uint8_t j = 0; j < zonesNum; j++)
  {
    if (RV_SUCCESS != SW_GetZoneConfig(&zone, j))
    {
      DBG_ERR(REST_MGR, "Unable to get zone#%d config data", j + 1);
      return REST_API_FAIL;
    }
    JSON_Start_ArrayObject();
    JSON_Add_Field_String("displayedName", zone.displayedName);
    JSON_Add_Field_Integer("type", zone.type);
    JSON_Add_Field_Integer("uniqueID", zone.uniqueID);
    JSON_Add_Field_Integer("numberOfLevels", zone.numberOfLevels);
    for (uint8_t i = 0; i < zone.numberOfLevels; i++)
    {
      snprintf(name, sizeof(name), "level%d", i + 1);
      JSON_Add_Field_Integer(name, zone.level[i]);
    }
    JSON_End_Object();
  }
  JSON_End_Array();
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseZoneConfigJsonAndProcess(char *json)
{
  char *singleObject = NULL;
  int value = 0;
  char name[16] = {0};
  zone_config_t zone;

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  for (uint8_t j = 0; j < SW_MAX_ZONES_NUM; j++)
  {
    if (0 == JSON_Extract_Object_From_Arr(json, "Zones", &singleObject, j + 1))
	{
      break;
	}

    if (0 == JSON_Extract_String(singleObject, "displayedName", zone.displayedName, sizeof(zone.displayedName)))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'displayedName' field from received json");
      return REST_API_FAIL;
    }

    if (0 == JSON_Extract_Integer(singleObject, "type", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'type' field from received json");
      return REST_API_FAIL;
    }
    zone.type = value;

    if (0 == JSON_Extract_Integer(singleObject, "uniqueID", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'uniqueID' field from received json");
      return REST_API_FAIL;
    }
    zone.uniqueID = value;

    if (0 == JSON_Extract_Integer(singleObject, "numberOfLevels", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'numberOfLevels' field from received json");
      return REST_API_FAIL;
    }
    zone.numberOfLevels = value;

    if (zone.numberOfLevels > SW_MAX_NUMBER_OF_LEVELS)
    {
      DBG_ERR(REST_MGR, "Too large numberOfLevels value received");
    }

    for (uint8_t i = 0; i < zone.numberOfLevels; i++)
    {
      snprintf(name, sizeof(name), "level%d", i + 1);

      if (0 == JSON_Extract_Integer(singleObject, name, &value))
      {
        DBG_ERR(REST_MGR, "Unable to parse 'level%d' field from received json", i + 1);
        return REST_API_FAIL;
      }
      zone.level[i] = value;
    }

	if (RV_SUCCESS != SW_SetZoneConfig(&zone, j))
	{
	  DBG_ERR(REST_MGR, "Unable to set zone#%d config data", j + 1);
	  return REST_API_FAIL;
	}
  }

  return REST_API_OK;
}

restApiStatus_t RM_GenerateSceneConfigJson(char **json)
{
  scene_config_t scene;
  uint8_t scenesNum = 0;

  if (RV_SUCCESS != SW_GetSceneNum(&scenesNum))
  {
    DBG_ERR(REST_MGR, "Unable to get number of scenes");
    return REST_API_FAIL;
  }

  JSON_Start();
  JSON_Start_Array("Scenes");
  for (uint8_t j = 0; j < scenesNum; j++)
  {
    if (RV_SUCCESS != SW_GetSceneConfig(&scene, j))
    {
      DBG_ERR(REST_MGR, "Unable to get zone#%d config data", j);
      return REST_API_FAIL;
    }
    JSON_Start_ArrayObject();
    JSON_Add_Field_String("displayedName", scene.displayedName);
    JSON_Add_Field_Integer("uniqueID", scene.uniqueID);
    JSON_End_Object();
  }
  JSON_End_Array();
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseSceneConfigJsonAndProcess(char *json)
{
  char *singleObject = NULL;
  int value = 0;
  scene_config_t scene;

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  for (uint8_t j = 0; j < SW_MAX_SCENES_NUM; j++)
  {
    if (0 == JSON_Extract_Object_From_Arr(json, "Scenes", &singleObject, j + 1))
	{
      break;
	}

    if (0 == JSON_Extract_String(singleObject, "displayedName", scene.displayedName, sizeof(scene.displayedName)))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'displayedName' field from received json");
      return REST_API_FAIL;
    }

    if (0 == JSON_Extract_Integer(singleObject, "uniqueID", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'uniqueID' field from received json");
      return REST_API_FAIL;
    }
    scene.uniqueID = value;

	if (RV_SUCCESS != SW_SetSceneConfig(&scene, j))
	{
	  DBG_ERR(REST_MGR, "Unable to set scene#%d config data", j + 1);
	  return REST_API_FAIL;
	}
  }

  return REST_API_OK;
}

restApiStatus_t RM_GenerateSystemConfigJson(char **json)
{
  system_config_t system;

  if (RV_SUCCESS != SW_GetSystemConfig(&system))
  {
    DBG_ERR(REST_MGR, "Unable to get system config data");
    return REST_API_FAIL;
  }

  JSON_Start();
  JSON_Add_Field_String("backgroundImage", system.backgroundImage);
  JSON_Add_Field_Integer("panelBrightness", system.panelBrightness);
  JSON_Add_Field_Integer("sleepAfter", system.sleepAfter);
  JSON_Add_Field_Integer("screenWaitDelay", system.screenWaitDelay);
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseSystemConfigJsonAndProcess(char *json)
{
  int value = 0;
  system_config_t system;

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  if (0 == JSON_Extract_String(json, "backgroundImage", system.backgroundImage, sizeof(system.backgroundImage)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'backgroundImage' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_Integer(json, "panelBrightness", &value))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'panelBrightness' field from received json");
    return REST_API_FAIL;
  }
  system.panelBrightness = value;

  if (0 == JSON_Extract_Integer(json, "sleepAfter", &value))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'sleepAfter' field from received json");
    return REST_API_FAIL;
  }
  system.sleepAfter = value;

  if (0 == JSON_Extract_Integer(json, "screenWaitDelay", &value))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'screenWaitDelay' field from received json");
    return REST_API_FAIL;
  }
  system.screenWaitDelay = value;

  if (RV_SUCCESS != SW_SetSystemConfig(&system))
  {
    DBG_ERR(REST_MGR, "Unable to set system config data");
    return REST_API_FAIL;
  }

  return REST_API_OK;
}

restApiStatus_t RM_ParseZoneStatusJsonAndProcess(char *json)
{
  char *singleObject = NULL;
  int value = 0;
  zone_status_t zoneStatus;

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  for (uint8_t j = 0; j < SW_MAX_ZONES_NUM; j++)
  {
    if (0 == JSON_Extract_Object_From_Arr(json, "zoneStatus", &singleObject, j + 1))
	{
      break;
	}

    if (0 == JSON_Extract_Integer(singleObject, "uniqueID", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'uniqueID' field from received json");
      return REST_API_FAIL;
    }
    zoneStatus.uniqueID = value;

    if (0 == JSON_Extract_Integer(singleObject, "level", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'level' field from received json");
      return REST_API_FAIL;
    }
    zoneStatus.level = value;

    if (0 == JSON_Extract_Integer(singleObject, "state", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'state' field from received json");
      return REST_API_FAIL;
    }
    zoneStatus.state = value;

    if (0 == JSON_Extract_Integer(singleObject, "mode", &value))
    {
      DBG_ERR(REST_MGR, "Unable to parse 'mode' field from received json");
      return REST_API_FAIL;
    }
    zoneStatus.mode = value;

	if (RV_SUCCESS != SW_SetZoneStatus(&zoneStatus, j))
	{
	  DBG_ERR(REST_MGR, "Unable to set zone#%d status data", j + 1);
	  return REST_API_FAIL;
	}
  }

  return REST_API_OK;
}

restApiStatus_t RM_ParseSceneStatusJsonAndProcess(char *json)
{
  int status = 0;

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  if (0 == JSON_Extract_Integer(json, "activeScene", &status))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'activeScene' field from received json");
    return REST_API_FAIL;
  }

  if (RV_SUCCESS != SW_SetSceneStatus((uint8_t)status))
  {
    DBG_ERR(REST_MGR, "Unable to set scene status data");
    return REST_API_FAIL;
  }

  return REST_API_OK;
}

restApiStatus_t RM_GenerateStoredApplicationsJson(char **json)
{
  fw_settings_t *fw_settings = NULL;
  char timeStamp[16] = {0};
  uint32_t loadFlag = 0;

  FW_GetSettingsPtr(&fw_settings);
  loadFlag = (fw_settings->newApp.useNextTime == FS_OK) ? 1 : 0;

  JSON_Start();

  JSON_Start_Object("slot1");
  JSON_Add_Field_Integer("majorRelease", (int) (fw_settings->bootArea[0].version.vMajor));
  JSON_Add_Field_Integer("minorRelease", (int) (fw_settings->bootArea[0].version.vMinor));
  JSON_Add_Field_Integer("buildRelease", (int) (fw_settings->bootArea[0].version.vBuild));
  JSON_Add_Field_Integer("bytes", (int) (fw_settings->bootArea[0].size));
  strncpy(timeStamp, fw_settings->bootArea[0].timeStamp, sizeof(timeStamp));
  JSON_Add_Field_String("timestamp", timeStamp);
  /* TODO: Check if it should be 1 and -1 or 1 and 0 */
  JSON_Add_Field_Integer("valid", (int)((fw_settings->bootArea[0].valid == FS_OK) ? 1 : -1));
  JSON_End_Object();

  JSON_Start_Object("slot2");
  JSON_Add_Field_Integer("majorRelease", (int) (fw_settings->bootArea[1].version.vMajor));
  JSON_Add_Field_Integer("minorRelease", (int) (fw_settings->bootArea[1].version.vMinor));
  JSON_Add_Field_Integer("buildRelease", (int) (fw_settings->bootArea[1].version.vBuild));
  JSON_Add_Field_Integer("bytes", (int) (fw_settings->bootArea[1].size));
  strncpy(timeStamp, fw_settings->bootArea[1].timeStamp, sizeof(timeStamp));
  JSON_Add_Field_String("timestamp", timeStamp);
  /* TODO: Check if it should be 1 and -1 or 1 and 0 */
  JSON_Add_Field_Integer("valid", (int)((fw_settings->bootArea[1].valid == FS_OK) ? 1 : -1));
  JSON_End_Object();

  JSON_Start_Object("currentApp");
  JSON_Add_Field_Integer("bytes", (int) (fw_settings->internalApp.size));
  JSON_Add_Field_Integer("knownCRC", (int) ((fw_settings->internalApp.size) ? 1 : 0));
  JSON_Add_Field_Integer("bootError", (int) ((fw_settings->internalApp.valid == FS_OK) ? 1 : -1));
  JSON_End_Object();

  JSON_Start_Object("bootCode");
  JSON_Add_Field_Integer("majorRelease", fw_settings->bootCodeVer.vMajor);
  JSON_Add_Field_Integer("minorRelease", fw_settings->bootCodeVer.vMinor);
  JSON_Add_Field_Integer("buildRelease", fw_settings->bootCodeVer.vBuild);
  JSON_End_Object();

  JSON_Start_Object("nextBoot");
  JSON_Add_Field_Integer("loadFlag", (int) loadFlag);
  JSON_Add_Field_Integer("majorRelease", (int) (fw_settings->newApp.version.vMajor));
  JSON_Add_Field_Integer("minorRelease", (int) (fw_settings->newApp.version.vMinor));
  JSON_Add_Field_Integer("buildRelease", (int) (fw_settings->newApp.version.vBuild));
  JSON_End_Object();

  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_GenerateBootErrorJson(char **json)
{
  fw_settings_t *fw_settings = NULL;
  crc_err_t crcErr = CRC_NO_ERROR;

  FW_GetSettingsPtr(&fw_settings);

  if (fw_settings->internalApp.valid == FS_NOT_OK)
  {
	crcErr = CRC_ERROR_INTERNAL;
  }

  if (fw_settings->bootArea[0].valid == FS_NOT_OK)
  {
	crcErr = CRC_ERROR_SLOT1;
  }

  if (fw_settings->bootArea[1].valid == FS_NOT_OK)
  {
	crcErr = CRC_ERROR_SLOT2;
  }

  JSON_Start();
  JSON_Add_Field_Integer("value", (int) (crcErr));
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseBootErrorJsonAndProcess(char *json)
{
  int errVal = 0;
  fw_settings_t *fw_settings = NULL;

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  FW_GetSettingsPtr(&fw_settings);

  if (0 == JSON_Extract_Integer(json, "value", &errVal))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'value' field from received json");
    return REST_API_FAIL;
  }

  if ((CRC_NO_ERROR == errVal) || (CRC_TABLE_CORRUPT == errVal) || (CRC_ERROR_SLOT1 == errVal)
		  || (CRC_ERROR_SLOT2 == errVal) || (CRC_ERROR_INTERNAL == errVal))
  {
    if (CRC_NO_ERROR == errVal)
    {
      fw_settings->bootArea[0].valid = FS_OK;
      fw_settings->bootArea[1].valid = FS_OK;
      fw_settings->internalApp.valid = FS_OK;
      fw_settings->newApp.valid = FS_OK;
      DBG_LOG(REST_MGR, "Boot error flags has been cleared");
    }
	else if (CRC_ERROR_SLOT1 == errVal)
	{
      fw_settings->bootArea[0].valid = FS_NOT_OK;
	}
	else if (CRC_ERROR_SLOT2 == errVal)
	{
      fw_settings->bootArea[1].valid = FS_NOT_OK;
	}
	else if (CRC_ERROR_INTERNAL == errVal)
	{
	  fw_settings->internalApp.valid = FS_NOT_OK;
	}
  }
  else
  {
	DBG_ERR(REST_MGR, "Incorrect Boot error parameter received");
	return REST_API_FAIL;
  }

  /* TODO: Do we need to save these flags to flash? */
  if (RV_SUCCESS != PS_Write(SYS_CONFIG_SECTOR_START, (const uint8_t *) fw_settings, sizeof(fw_settings_t)))
  {
    DBG_ERR(REST_MGR, "Failed to save FW configuration data to persistent storage");

    return REST_API_FAIL;
  }

  return REST_API_OK;
}

/* memType = 0 in case of external memory, memType = 1 in case of internal memory */
restApiStatus_t RM_GenerateMemoryValueJson(char **json, uint32_t addr, uint8_t memType)
{
  uint8_t value = 0;
  char valueStr[6] = {0};

  if (0 == memType)
  {
    /* External memory. Validate address range */
	if (addr <= FLASH_END_ADDR)
	{
	  if (RV_SUCCESS != PS_Read(addr, &value, 1))
	  {
		DBG_ERR(REST_MGR, "Failed to read single byte from external flash");

		return RV_FAILURE;
	  }
	}
	else
	{
	  DBG_ERR(REST_MGR, "Incorrect memory range for external flash");

	  return RV_FAILURE;
	}
  }
  else
  {
    /* Internal memory. Validate address range */
    if ((addr >= APPLICATION_MEMORY_START) && (addr < APPLICATION_MEMORY_END))
    {
    	value = *((uint8_t *)(addr));
	}
	else
	{
	  DBG_ERR(REST_MGR, "Incorrect memory range for internal flash");

	  return RV_FAILURE;
	}
  }

  sprintf(valueStr, "0x%02x", value);

  JSON_Start();
  JSON_Add_Field_String("value", valueStr);
  *json = JSON_End();

  return REST_API_OK;
}

restApiStatus_t RM_ParseMemoryChangeJsonAndProcess(char *json, uint8_t memType)
{
  char hexValue[12] = {0};
  uint32_t addr = 0;
  uint8_t value = 0;

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  if (0 == JSON_Extract_String(json, "address", hexValue, sizeof(hexValue)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'address' field from received json");
    return REST_API_FAIL;
  }

  addr = (uint32_t) strtol(hexValue, NULL, 16);

  if (0 == JSON_Extract_String(json, "value", hexValue, sizeof(hexValue)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'value' field from received json");
    return REST_API_FAIL;
  }

  value = (uint8_t) strtol(hexValue, NULL, 16);

  if (0 == memType)
  {
    /* External memory. Validate address range */
    if (addr <= FLASH_END_ADDR)
    {
	  /* TODO: Overcome erasing flash for addresses
	   * that are aliquot to sector size */
      if (RV_SUCCESS != PS_Write(addr, &value, 1))
	  {
	    DBG_ERR(REST_MGR, "Failed to write single byte to external flash");

	    return RV_FAILURE;
	  }
    }
    else
    {
  	  DBG_ERR(REST_MGR, "Incorrect memory range for external flash");

  	  return RV_FAILURE;
    }
  }
  else
  {
    /* Internal memory. Validate address range */
	if ((addr >= APPLICATION_MEMORY_START) && (addr < APPLICATION_MEMORY_END))
	{
	  /* TODO: This is something weird. Should be disabled later */
	  HAL_FLASH_Unlock();
	  if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr, value))
	  {
		DBG_ERR(REST_MGR, "Failed to program internal flash. Error: %lu",
		  	             HAL_FLASH_GetError());
	  }
	  HAL_FLASH_Lock();
	}
    else
	{
	  DBG_ERR(REST_MGR, "Incorrect memory range for internal flash");

	  return RV_FAILURE;
    }
  }

  return REST_API_OK;
}

/************************ Start of Firmware Update Related Part *****************************/

static restApiStatus_t sendUpdResultResponse(restApiStatus_t status)
{
  osTimerStop(fwUpdateTimerHandle);

  fwReceiveTimeout = 0;
  waitingForFWChunk = 0;

  if ((NULL == fwUpd_http_g) || (NULL == fwUpd_pcb_g))
  {
    DBG_ERR(REST_MGR, "Failed to send response. Not correct pcb and/or http settings");
	return 	REST_API_FAIL;
  }

  /* Disable keep alive option - connection will be closed soon */
  fwUpd_http_g->keepalive = 0;

  if(REST_API_OK == status)
  {
    /* All is OK. Give OK response */
    tcp_write(fwUpd_pcb_g, HTTP_OK_RESP, sizeof(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
  }
  else
  {
	tcp_write(fwUpd_pcb_g, HTTP_BAD_RESP, strlen(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
  }
  tcp_output(fwUpd_pcb_g);

  return REST_API_OK;
}

static restApiStatus_t requestFWDataChunk(void)
{
  if (NULL != fwUpd_pcb_g)
  {
    memset(restApiMsgBuf_g, '0', FW_UPD_CHUNK_SIZE);
    strcpy(restApiMsgBuf_g, "SENDCHUNK\n");

    fwReceiveTimeout = 0;
    waitingForFWChunk = 1;

    tcp_write(fwUpd_pcb_g, restApiMsgBuf_g, FW_UPD_CHUNK_SIZE, TCP_WRITE_FLAG_COPY);
    tcp_output(fwUpd_pcb_g);

    return REST_API_OK;
  }

  return REST_API_FAIL;
}

void fwUpdateTimerHandler(void const * argument)
{
  uint8_t state = 0;

  if (fwReceiveTimeout >= FW_UPD_TIMEOUT_MS)
  {
    sendUpdResultResponse(REST_API_FAIL);
    return;
  }

  if (waitingForFWChunk)
  {
    fwReceiveTimeout += FW_UPD_TIMER_POLL_PERIOD_MS;
	return;
  }

  /* Check if previous fw data chunk was completely saved at the external flash
   * If flash is busy, try again later */
  PS_FlashGetState(&state);
  if (0 == state)
  {
    /* Request next fw chunk */
    if (REST_API_OK != requestFWDataChunk())
    {
      sendUpdResultResponse(REST_API_FAIL);
    }
  }
}

static err_t updateFW_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  err_t ret_err;

  fwUpd_http_g = (struct http_state *)arg;

  if (p == NULL)
  {
    DBG_ERR(REST_MGR, "Received NULL pointer buffer");
    ret_err = ERR_OK;
  }
  else if(err != ERR_OK)
  {
    if (p != NULL)
    {
      pbuf_free(p);
    }
    ret_err = err;
    DBG_ERR(REST_MGR, "Error in updateFW_recv(): %d", ret_err);

    sendUpdResultResponse(REST_API_FAIL);
  }
  else
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);
    if (p->len != FW_UPD_CHUNK_SIZE)
    {
      DBG_ERR(REST_MGR, "Incorrect chunk size received = %d", p->len);

      sendUpdResultResponse(REST_API_FAIL);
    }
    else
    {
      if (0 == strncmp(p->payload, FW_UPD_TERMINATOR, strlen(FW_UPD_TERMINATOR)))
      {
      	/* Stop timer to avoid next requests */
      	osTimerStop(fwUpdateTimerHandle);

      	if (RV_SUCCESS == FW_FinalizeUpdate())
      	{
      	  DBG_LOG(REST_MGR, "All firmware has been received. Waiting for reboot request");
      	  sendUpdResultResponse(REST_API_OK);

      	  //das_g.state = DS_UPDATE;
      	  //osTimerStart(delayedActionTimerHandle, pdMS_TO_TICKS(1000 * DELAYED_ACTION_TIMEOUT_SEC));
      	}
      	else
      	{
      	  DBG_ERR(REST_MGR, "Update failed. Inform server");
      	  sendUpdResultResponse(REST_API_FAIL);
      	}
      }
      else
      {
        if (RV_SUCCESS != FW_SaveDataChunk((uint8_t *)(p->payload), FW_UPD_CHUNK_SIZE))
        {
          DBG_ERR(REST_MGR, "Failed to save firmware data chunk");
          sendUpdResultResponse(REST_API_FAIL);
        }
        fwReceiveTimeout = 0;
        waitingForFWChunk = 0;
      }
    }

	/* Free pbuf and do nothing */
	pbuf_free(p);
	ret_err = ERR_OK;
  }

  return ret_err;
}

restApiStatus_t RM_SetupFirmwareDownloadCommChannel(struct tcp_pcb *pcb, char *json)
{
  uint32_t majorVer = 0;
  uint32_t minorVer = 0;
  uint32_t buildVer = 0;
  uint32_t bytesLen = 0;
  char timeStamp[16] = {0};
  uint32_t slotNum = 0;

  if ((!json) || (NULL == pcb))
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  if (0 == JSON_Extract_Integer(json, "major", (int *)&majorVer))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'major' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_Integer(json, "minor", (int *)&minorVer))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'minor' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_Integer(json, "build", (int *)&buildVer))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'build' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_Integer(json, "bytes", (int *)&bytesLen))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'bytes' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_String(json, "time", timeStamp, sizeof(timeStamp)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'time' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_Integer(json, "slot", (int *)&slotNum))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'slot' field from received json");
    return REST_API_FAIL;
  }

  fwUpd_pcb_g = pcb;

  /* Replace standard httpd receive callback function by custom one */
  tcp_recv(pcb, updateFW_recv);

  /* Prepare for firmware update */
  if (RV_SUCCESS !=  FW_StartUpdate(majorVer, minorVer, buildVer, bytesLen, timeStamp, slotNum))
  {
    return REST_API_FAIL;
  }

  /* If everything is OK - start firmware download */
  if (REST_API_OK != requestFWDataChunk())
  {
    return REST_API_FAIL;
  }

  osTimerStart(fwUpdateTimerHandle, pdMS_TO_TICKS(FW_UPD_TIMER_POLL_PERIOD_MS));

  return REST_API_OK;
}

/************************ End of Firmware Update Related Part *****************************/

restApiStatus_t RM_ExtFlashInitProcess(void)
{
  /* Assume that firmware setting require more memory than network settings,
   * so buffer will be created with bigger size. Erasing of external flash
   * blocks is not working as expected.
   * It's better not to write/erase without using PS queue */
  char emptyBuf[sizeof(fw_settings_t)];

  memset(emptyBuf, 0xFF, sizeof(fw_settings_t));

  if (RV_SUCCESS != PS_Write(SYS_CONFIG_SECTOR_START, (const uint8_t *) emptyBuf, sizeof(fw_settings_t)))
  {
    DBG_ERR(REST_MGR, "Failed to save FW configuration data to persistent storage");

    return REST_API_FAIL;
  }

  if (RV_SUCCESS != PS_Write(NETWORK_CONFIG_SECTOR_START, (const uint8_t *) emptyBuf, sizeof(networkSettings_t)))
  {
    DBG_ERR(ETH_MGR, "Failed to write settings to flash memory");
    return RV_FAILURE;
  }

  return REST_API_OK;
}

restApiStatus_t RM_ParceRebootJsonAndProcess(struct tcp_pcb *pcb, char *json)
{
  fw_settings_t *fw_settings = NULL;
  uint32_t majorVer = 0;
  uint32_t minorVer = 0;
  uint32_t buildVer = 0;
  uint8_t slotNum = 0xFF;

  if ((!json) || (NULL == pcb))
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  FW_GetSettingsPtr(&fw_settings);

  if (0 == JSON_Extract_Integer(json, "major", (int *)&majorVer))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'major' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_Integer(json, "minor", (int *)&minorVer))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'minor' field from received json");
    return REST_API_FAIL;
  }

  if (0 == JSON_Extract_Integer(json, "build", (int *)&buildVer))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'build' field from received json");
    return REST_API_FAIL;
  }

  DBG_LOG(REST_MGR, "Looking for %lu.%lu.%lu version...", majorVer, minorVer, buildVer);

  /* Look after this firmware version on the external flash and detect slot.
   * If slot not found leave it's number as 0xFF */
  if ((fw_settings->bootArea[0].version.vMajor == majorVer) &&
	  (fw_settings->bootArea[0].version.vMinor == minorVer) &&
	  (fw_settings->bootArea[0].version.vBuild == buildVer))
  {
	slotNum = 1;
  }
  else if ((fw_settings->bootArea[1].version.vMajor == majorVer) &&
		  (fw_settings->bootArea[1].version.vMinor == minorVer) &&
		  (fw_settings->bootArea[1].version.vBuild == buildVer))
  {
	slotNum = 2;
  }

  if (0xFF == slotNum)
  {
    tcp_write(pcb, HTTP_BAD_RESP, strlen(HTTP_BAD_RESP), TCP_WRITE_FLAG_COPY);
    tcp_output(pcb);
    DBG_ERR(REST_MGR, "Unable to find slot with specified firmware version");
    fw_settings->newApp.useNextTime = FS_NOT_OK;
    if (RV_SUCCESS != FW_SaveUpdateSettings())
    {
      return REST_API_FAIL;
    }
    DBG_ERR(REST_MGR, "Internal app will be loaded");
  }
  else
  {
    tcp_write(pcb, HTTP_OK_RESP, strlen(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
    tcp_output(pcb);

    /* Make changes in firmware info section on the external flash */
    fw_settings->newApp.useNextTime = FS_OK;
    fw_settings->newApp.version.vMajor = majorVer;
    fw_settings->newApp.version.vMinor = minorVer;
    fw_settings->newApp.version.vBuild = buildVer;

    if (RV_SUCCESS != FW_SaveUpdateSettings())
    {
      return REST_API_FAIL;
    }
  }

  DBG_ERR(REST_MGR, "Reboot in 2 seconds...");

  das_g.state = DS_REBOOT;
  osTimerStart(delayedActionTimerHandle, pdMS_TO_TICKS(1000 * DELAYED_ACTION_TIMEOUT_SEC));

  return REST_API_OK;
}

restApiStatus_t RM_ResetProcess(struct tcp_pcb *pcb)
{
  fw_settings_t *fw_settings = NULL;

  /* Make changes in firmware info section on the external flash to skip all updates */
  FW_GetSettingsPtr(&fw_settings);
  fw_settings->newApp.useNextTime = FS_NOT_OK;
  //fw_settings->newApp.valid = FS_NOT_OK;

  if (RV_SUCCESS != FW_SaveUpdateSettings())
  {
	DBG_ERR(REST_MGR, "Failed to process reset action. Flash error happen");
    return REST_API_FAIL;
  }

  DBG_ERR(REST_MGR, "Reset in 2 seconds...");

  tcp_write(pcb, HTTP_OK_RESP, strlen(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY);
  tcp_output(pcb);

  das_g.state = DS_RESET;
  osTimerStart(delayedActionTimerHandle, pdMS_TO_TICKS(1000 * DELAYED_ACTION_TIMEOUT_SEC));

  return REST_API_OK;
}

/* 1 - ping message; 0 - message with data */
restApiStatus_t RM_GenerateSwitchDataAndSend(uint8_t keyPresType, uint8_t uniqueID, uint8_t level, uint8_t msgType)
{
  char *respJson = NULL;

  if (NULL == switch_pcb_g)
  {
    DBG_ERR(REST_MGR, "Null pointer pcb received");
    return REST_API_NULL_PTR;
  }

  if (msgType)
  {
	/* TODO: Send ping message. Which format we should use? For now - empty json */
    JSON_Start();
    respJson = JSON_End();
  }
  else
  {
    JSON_Start();
	JSON_Add_Field_Integer("keypressType", keyPresType);
	JSON_Add_Field_Integer("uniqueID", uniqueID);
	JSON_Add_Field_Integer("level", level);
	respJson = JSON_End();
  }

  tcp_write(switch_pcb_g, respJson, strlen(respJson) + 1, TCP_WRITE_FLAG_COPY);
  tcp_output(switch_pcb_g);

  return REST_API_OK;
}

restApiStatus_t RM_SetupSwitchDataCommChannel(struct tcp_pcb *pcb)
{
  if (NULL != switch_pcb_g)
  {
    /* We already have persistent switch data connection:
	 * close existing connection and open new one */
	osTimerStop(switchPingTimerHandle);

    if (ERR_MEM == tcp_close(switch_pcb_g))
    {
      /* TODO: Try to close connection again */
      DBG_ERR(REST_MGR, "Failed to close connection");
    }

    DBG_LOG(REST_MGR, "Old switch data connection deleted. Open new one...");
  }

  switch_pcb_g = pcb;

  /* Turn on TCP Keepalive for the given pcb */
  switch_pcb_g->so_options |= SOF_KEEPALIVE;

  /* Set the time between keepalive messages in milli-seconds */
  switch_pcb_g->keep_intvl = 3000; /* 3 seconds */

  /* Send first ping message immediately */
  tcp_write(switch_pcb_g, HTTP_OK_RESP, strlen(HTTP_OK_RESP), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE);
  RM_GenerateSwitchDataAndSend(0, 0, 0, 1);

  /* Start timer to send ping messages and protect connection from closing */
  osTimerStart(switchPingTimerHandle, pdMS_TO_TICKS(1000 * SWITCH_PING_SEC));

  return REST_API_OK;
}

void switchPingTimerHandler(void const * argument)
{
  RM_GenerateSwitchDataAndSend(0, 0, 0, 1);
}

void delayedActionTimerHandler(void const * argument)
{
  switch (das_g.state)
  {
    case DS_IP_SET:
	  if (das_g.settings->ipSet.dhcpEnable)
	  {
        dhcp_start(&gnetif);
	  }
	  else
	  {
        /* Check if used dhcp before and stop it */
		if (NULL != ((struct dhcp*)netif_get_client_data(&gnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP)))
		{
		   dhcp_release(&gnetif);
		   dhcp_stop(&gnetif);
		}
        netif_set_addr(&gnetif, &(das_g.settings->ipSet.ipaddr), &(das_g.settings->ipSet.netmask), &(das_g.settings->ipSet.gw));
	  }
      break;

    case DS_MAC_SET:
    case DS_REBOOT:
    case DS_RESET:
    case DS_UPDATE:
    	NVIC_SystemReset();
      break;

    default:
    	DBG_ERR(REST_MGR, "Delayed action timer has been started without need");
      break;
  }
}

restApiStatus_t RM_ParseClockJsonAndProcess(char *json)
{
  char stringValue[16] = {0};
  char *p = NULL;
  int extractValue = 0;
  uint8_t hour = 0, minute = 0, second = 0;
  uint8_t day = 0, month = 0, year = 0, dayOfTheWeek = 0;
  uint8_t sunriseHour = 0, sunriseMin = 0;
  uint8_t sunsetHour = 0, sunsetMin = 0;
  year = 19; /* We don't receive year from supervisor. Just hardcode year value */

  if (!json)
  {
    DBG_ERR(REST_MGR, "Null pointer received");
    return REST_API_NULL_PTR;
  }

  if (0 == JSON_Extract_String(json, "time", stringValue, sizeof(stringValue)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'time' field from received json");
    return REST_API_FAIL;
  }

  p = (char *) stringValue;
  hour = atoi(p);
  p = strchr(p, ':');
  minute = atoi(++p);
  p = strchr(p, ':');
  second = atoi(++p);

  if (0 == JSON_Extract_Integer(json, "dayOfWeek", &extractValue))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'dayOfWeek' field from received json");
    return REST_API_FAIL;
  }
  dayOfTheWeek = (uint8_t)extractValue;

  if (0 == JSON_Extract_Integer(json, "month", &extractValue))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'month' field from received json");
    return REST_API_FAIL;
  }
  month = (uint8_t)extractValue;

  if (0 == JSON_Extract_Integer(json, "dayOfMonth", &extractValue))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'dayOfMonth' field from received json");
    return REST_API_FAIL;
  }
  day = (uint8_t)extractValue;

  if (0 == JSON_Extract_String(json, "sunrise", stringValue, sizeof(stringValue)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'sunrise' field from received json");
    return REST_API_FAIL;
  }

  p = (char *) stringValue;
  sunriseHour = atoi(p);
  p = strchr(p, ':');
  sunriseMin = atoi(++p);

  if (0 == JSON_Extract_String(json, "sunset", stringValue, sizeof(stringValue)))
  {
    DBG_ERR(REST_MGR, "Unable to parse 'sunset' field from received json");
    return REST_API_FAIL;
  }

  p = (char *) stringValue;
  sunsetHour = atoi(p);
  p = strchr(p, ':');
  sunsetMin = atoi(++p);

  if (RV_SUCCESS != SW_ClockConfig(hour, minute, second, day, month, year,
		  dayOfTheWeek, sunriseHour, sunriseMin, sunsetHour, sunsetMin))
  {
    DBG_ERR(REST_MGR, "Unable to set time/date parameters");
	return REST_API_FAIL;
  }

  return REST_API_OK;
}

#if REST_OUT_RQST_ENABLE
restApiStatus_t callbackExample(uint16_t code, char *header, uint16_t headerLen, uint8_t *data, uint16_t dataLen)
{
  /* Just print all received message parts */
  DBG_LOG(REST_MGR, "Result code: %d", code);
  DBG_LOG(REST_MGR, "Header: \r\n%s", header);
  DBG_LOG(REST_MGR, "Header length: %d", headerLen);

  DBG_LOG(REST_MGR, "Data: \r\n%s", data);
  DBG_LOG(REST_MGR, "Data length: %d", dataLen);

  return REST_API_OK;
}

static void RestApiTask(void *arg)
{
  osEvent evt;
  restApiMsg_t *mail = NULL;

  /* Initiate code for restApiTask */


  /* Infinite loop */
  while(1)
  {
	/* Is there other request in progress? */
    if ((NULL == cs_g) && (NULL == client_pcb_g))
    {
      /* Clear previous message memory */
      if (mail)
      {
        if (NULL != (mail->payload))
        {
          free(mail->payload);
        }
    	osMailFree(restMsgBox_g, mail);
    	mail = NULL;
      }

      evt = osMailGet(restMsgBox_g, 10);
      if (osEventMail == evt.status)
      {
        mail = (restApiMsg_t *) evt.value.p;
        if (0 != mail)
    	{
          if (((mail->type) == REST_API_OUT_HTTP_POST) || ((mail->type) == REST_API_OUT_HTTP_PUT) || ((mail->type) == REST_API_OUT_HTTP_GET))
          {
        	/* Handle outcoming REST api requests */
            if (REST_API_OK != sendMsgFromRESTApiQueue(mail))
            {
              DBG_ERR(REST_MGR, "Failed to open connection");
              tcp_client_connection_close(cs_g->pcb, cs_g); // TODO: Try again?
            }
          }
          else if (((mail->type) == REST_API_IN_HTTP_POST) || ((mail->type) == REST_API_IN_HTTP_PUT) || ((mail->type) == REST_API_IN_HTTP_GET))
          {
        	/* Handle incoming REST api requests */
        	/* TODO: parse and run appropriate callback */
          }
          else
          {
        	DBG_ERR(REST_MGR, "Unknown request type");
          }
    	}
      }
    }
    //osDelay(1);
  }
}
#endif

/* Should be run once at system startup after LwIP stack init.
 * Any request couldn't be send before REST initialization */
restApiStatus_t RM_InitRESTApi(void)
{
#if REST_OUT_RQST_ENABLE
  /* Create REST request queue */
  restMsgBox_g = osMailCreate(osMailQ(restMsgBox_g), NULL);
  if (0 == restMsgBox_g)
  {
    DBG_ERR(REST_MGR, "Failed to create REST mail queue");
    return REST_API_FAIL;
  }

  /* Create thread for REST api functionality */
  const osThreadDef_t os_thread_def = { "REST thread", (os_pthread)RestApiTask, osPriorityNormal, 0, REST_API_STACK_SIZE };
  if (NULL == osThreadCreate(&os_thread_def, NULL))
  {
    DBG_ERR(REST_MGR, "Failed to create REST thread");
    return REST_API_FAIL;
  }
#endif

  /* Create timer for persistent switch socket ping */
  /* TODO: Do we need this ping message to keep connection or we may let close it? */
  const osTimerDef_t timer_def = { .ptimer = switchPingTimerHandler };
  switchPingTimerHandle = osTimerCreate(&timer_def, osTimerPeriodic, 0);
  if (NULL == switchPingTimerHandle)
  {
    DBG_ERR(REST_MGR, "Failed to create switch ping timer");
    return REST_API_FAIL;
  }

  /* Create timer for delayed action proceed */
  const osTimerDef_t timer_def1 = { .ptimer = delayedActionTimerHandler };
  delayedActionTimerHandle = osTimerCreate(&timer_def1, osTimerOnce, 0);
  if (NULL == delayedActionTimerHandle)
  {
    DBG_ERR(REST_MGR, "Failed to create delayed action timer");
    return REST_API_FAIL;
  }

  /* Create timer for firmware update purposes */
  const osTimerDef_t timer_def2 = { .ptimer = fwUpdateTimerHandler };
  fwUpdateTimerHandle = osTimerCreate(&timer_def2, osTimerPeriodic, 0);
  if (NULL == fwUpdateTimerHandle)
  {
    DBG_ERR(REST_MGR, "Failed to create firmware update timer");
    return REST_API_FAIL;
  }

  /* Initialize settings pointer */
  LW_GetNetworkSettingsPtr(&das_g.settings);

  if (RV_SUCCESS != FW_CheckAtStartup())
  {
	DBG_ERR(REST_MGR, "Failed to read firmware update details from flash");
  }

  return REST_API_OK;
}

#if REST_OUT_RQST_ENABLE
restApiStatus_t RM_PutMsgToRESTApiQueue(restApiRqstType_t type, char *destIP, uint16_t destPort,
		                                char *uri, char *data, uint16_t dataLen, restApiResp_cb_t rest_cb)
{
  osStatus status = osOK;

  /* Check REST mail queue exist again to ensure correct api usage */
  if (0 == restMsgBox_g)
  {
    DBG_ERR(REST_MGR, "REST mail queue not created. Run initRESTApi() first");
    return REST_API_FAIL;
  }

  if(!destIP)
  {
    DBG_ERR(REST_MGR, "There is no destination IP parameter");
    return REST_API_NULL_PTR;
  }

  if(!destPort)
  {
    DBG_ERR(REST_MGR, "Destination port couldn't be zero. Set default: 80");
    destPort = 80;
  }

  if(!uri)
  {
    DBG_ERR(REST_MGR, "There is no URI/endpoint for request");
    return REST_API_NULL_PTR;
  }

  if (strlen(uri) >= REST_API_MAX_URI_LEN)
  {
    DBG_ERR(REST_MGR, "Uri for REST request is too long or no null terminator.");
    DBG_ERR(REST_MGR, "Try to increase REST_API_MAX_URI_LEN");
    return REST_API_FAIL;
  }

  if(!data)
  {
    dataLen = 0;
    /* TODO: We should support empty requests. For example GET.
     * But need to clarify requirement of empty POST and PUT */
    //DBG_ERR(REST_MGR, "Empty request");
    //return REST_API_NULL_PTR;
  }
  else if (dataLen > REST_API_MAX_MSG_LEN)
  {
    DBG_ERR(REST_MGR, "Too long message. Try to increase REST_API_MAX_MSG_LEN");
    return REST_API_FAIL;
  }

  /* Allocate memory for message */
  restApiMsg_t *mail = (restApiMsg_t*) osMailAlloc(restMsgBox_g, 100);
  if (0 == mail)
  {
    DBG_ERR(REST_MGR, "No memory for REST request, increase heap or/and stack size");
    return REST_API_NULL_PTR;
  }

  mail->type = type;
  mail->destIP = ipaddr_addr(destIP);
  mail->port = destPort;
  strncpy(mail->uri, uri, REST_API_MAX_URI_LEN);
  mail->cb = rest_cb;
  if(data)
  {
    mail->payload = (uint8_t *)malloc(dataLen + 1);
    if (NULL == (mail->payload))
    {
      DBG_ERR(REST_MGR, "No memory for request data, increase heap or/and stack size");
      return REST_API_NULL_PTR;
    }

	memset(mail->payload, 0x00, dataLen + 1);
    memcpy(mail->payload, data, dataLen);

    mail->payloadLen = dataLen;
  }
  else
  {
    mail->payload = NULL;
    mail->payloadLen = 0;
  }

  status = osMailPut(restMsgBox_g, mail);
  if(osOK != status)
  {
    DBG_ERR(REST_MGR, "Failed to add mail to REST requests queue. Error:%u", status);
    return REST_API_FAIL;
  }

  return REST_API_OK;
}

static restApiStatus_t sendMsgFromRESTApiQueue(restApiMsg_t *mail)
{
  err_t err = ERR_OK;

  ip_addr_t destIPaddr;
  client_pcb_g = tcp_new();

  if (client_pcb_g != NULL)
  {
    destIPaddr.addr = mail->destIP;

    /* Save context for future needs */
    apiContext_g = mail;

    /* Register error callback */
    tcp_err(client_pcb_g, tcp_client_err);

    /* Try to connect */
    err = tcp_connect(client_pcb_g, &destIPaddr, mail->port, tcp_client_connected);

    if (err != ERR_OK)
    {
      DBG_LOG(REST_MGR, "Unable to send: %d", err);
      return REST_API_FAIL;
    }
  }
  else
  {
    DBG_LOG(REST_MGR, "Not enough memory for new pcb block");
    return REST_API_NULL_PTR;
  }

  return REST_API_OK;
}

restApiStatus_t generateRequest(restApiMsg_t *rqst)
{
  char hostStr[REST_API_MAX_IP_LEN] = {0};

  if (!rqst)
  {
    DBG_ERR(REST_MGR, "Null pointer received in generate request function");
	return REST_API_NULL_PTR;
  }

  snprintf(hostStr, REST_API_MAX_IP_LEN, "%u.%u.%u.%u", (uint8_t)(rqst->destIP),
		  (uint8_t) (rqst->destIP >> 8), (uint8_t) (rqst->destIP >> 16), (uint8_t) (rqst->destIP >> 24));

  switch (rqst->type)
  {
	case REST_API_OUT_HTTP_POST:
		snprintf(restApiMsgBuf_g, REST_API_MAX_BUF_LEN, POST_CMD_BASE, rqst->uri, HOST_FIELD,
				hostStr, CONTENT_TYPE_FIELD, CONTENT_LEGTH_FIELD, strlen((char *)rqst->payload), (char *)rqst->payload);
		break;

	case REST_API_OUT_HTTP_PUT:
		snprintf(restApiMsgBuf_g, REST_API_MAX_BUF_LEN, PUT_CMD_BASE, rqst->uri, HOST_FIELD,
				hostStr, CONTENT_TYPE_FIELD, CONTENT_LEGTH_FIELD, strlen((char *)rqst->payload), (char *)rqst->payload);
		break;

	case REST_API_OUT_HTTP_GET:
		snprintf(restApiMsgBuf_g, REST_API_MAX_BUF_LEN, GET_CMD_BASE, rqst->uri, HOST_FIELD, hostStr);
		break;

	default:
		DBG_ERR(REST_MGR, "Incorrect request type: %d", rqst->type);
		return REST_API_FAIL;
		break;
  }

  DBG_LOG(REST_MGR, "Request:\r\n%s", restApiMsgBuf_g);

  return REST_API_OK;
}

/************************ LwIP callback functions ***************************/

static void tcp_client_err(void *arg, err_t err)
{
  DBG_ERR(REST_MGR, "LWIP error: %d", err);

  if ((ERR_ABRT == err) || (ERR_RST == err))
  {
    tcp_client_connection_close(cs_g->pcb, cs_g); // TODO: Try again?
  }
  /* TODO: Handle communication errors */
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  struct client_struct *es = NULL;

  if (err == ERR_OK)
  {
    es = (struct client_struct *)mem_malloc(sizeof(struct client_struct));
	if (es != NULL)
	{
#ifdef DEBUG
	  DBG_LOG(REST_MGR, "Connected: %d", connNum_g);
#endif
	  es->state = ES_CONNECTED;
	  es->pcb = tpcb;
	  /* Generate request string data */
	  if (REST_API_OK != generateRequest(apiContext_g))
	  {
		DBG_ERR(REST_MGR, "Failed to generate request");
		return ERR_MEM;
	  }
	  es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen(restApiMsgBuf_g), PBUF_RAM);  //PBUF_POOL +1 to len

	  if (es->p_tx)
	  {
	    /* Copy data to pbuf */
	    pbuf_take(es->p_tx, restApiMsgBuf_g, strlen(restApiMsgBuf_g)); // + 1 to len
	    /* Pass newly allocated es structure as argument to tpcb */
	    tcp_arg(tpcb, es);
	    /* Initialize LwIP tcp_recv callback function */
	    tcp_recv(tpcb, tcp_client_recv);
	    /* Initialize LwIP tcp_sent callback function */
	    tcp_sent(tpcb, tcp_client_sent);
	    /* Initialize LwIP tcp_poll callback function */
	    tcp_poll(tpcb, tcp_client_poll, 1);
	    /* Send data */
	    tcp_client_send(tpcb, es);

	    /* Save client structure pointer for connection status tracking */
	    cs_g = es;

	    return ERR_OK;
	  }
	  else
	  {
	    DBG_ERR(REST_MGR, "Out of memory, can't allocate memory for pbuf");
	    return ERR_MEM;
	  }
	}
	else
	{
	  /* Close connection */
	  tcp_client_connection_close(tpcb, es);
	  DBG_ERR(REST_MGR, "Out of memory, can't allocate memory for client structure");
	  /* Return memory allocation error */
	  return ERR_MEM;
	}
  }
  else
  {
	DBG_ERR(REST_MGR, "Connection failed");
    /* close connection */
    tcp_client_connection_close(tpcb, es);
  }

  return err;
}

static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client_struct * es)
{
  /* Remove callback info */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  if ((es->p_tx) != NULL)
  {
	pbuf_free(es->p_tx);
  }

  if (es != NULL)
  {
	mem_free(es);
  }

  /* Close TCP connection */
  tcp_close(tpcb);

  /* Unblock queue to proceed with next message */
  client_pcb_g = NULL;
  cs_g = NULL;
}

void receiveComplete(void *arg)
{
  struct client_struct *es = (struct client_struct *)arg;
  uint16_t code = 0;
  uint16_t headerLen = 0;
  char *ptr = NULL;

  /* TODO: We assume response code is placed right after first space symbol. Revisit later */
  code = (uint16_t)(atoi(strchr(restApiMsgBuf_g, ' ')));

  headerLen = (uint16_t)(strstr(restApiMsgBuf_g, "\r\n\r\n") - restApiMsgBuf_g);

  ptr = restApiMsgBuf_g + headerLen + strlen("\r\n\r\n");
  restApiMsgBuf_g[headerLen] = '\0';

  if (REST_API_OK != apiContext_g->cb(code, restApiMsgBuf_g, headerLen, (uint8_t *)ptr, restApiMsgBufPtr_g - ptr))
  {
    /* TODO: Should we handle this case or just ignore? */
    DBG_ERR(REST_MGR, "Callback function returned error");
  }
  es->state = ES_CLOSING;
}

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct client_struct *es = (struct client_struct *)arg;
  err_t ret_err;

  if (p == NULL)
  {
    /* Prepare connection for close */
    if (NULL != apiContext_g->cb)
    {
      receiveComplete(arg);
    }
    DBG_LOG(REST_MGR, "Prepare for close connection");

    es->state = ES_CLOSING;
    ret_err = ERR_OK;
  }
  else if(err != ERR_OK)
  {
    if (p != NULL)
    {
      pbuf_free(p);
    }
    ret_err = err;
    DBG_ERR(REST_MGR, "Error in tcp_client_recv(): %d", ret_err);
  }
  else if(es->state == ES_CONNECTED)
  {
    //tcp_recved(tpcb, p->tot_len);
    es->p_tx = p;

	/* Check if we could fit received data */
	if ((restApiMsgBufPtr_g + es->p_tx->len) >= (restApiMsgBuf_g + REST_API_MAX_BUF_LEN))
	{
	  /* Data don't fit - reserve one byte for null terminator */
	  memcpy(restApiMsgBufPtr_g, es->p_tx->payload, REST_API_MAX_BUF_LEN - (restApiMsgBufPtr_g - restApiMsgBuf_g) - 1);
	  restApiMsgBufPtr_g = &(restApiMsgBuf_g[REST_API_MAX_BUF_LEN - 1]);
	  *restApiMsgBufPtr_g = '\0';
	  DBG_ERR(REST_MGR, "Some data will be lost. Increase REST_API_MAX_BUF_LEN to avoid this!");
	}
	else
	{
	  memcpy(restApiMsgBufPtr_g, es->p_tx->payload, es->p_tx->len);
	  restApiMsgBufPtr_g += es->p_tx->len;
	  *restApiMsgBufPtr_g = '\0';
	  //DBG_LOG(REST_MGR, "We received data. Fit. Bytes = %d", es->p_tx->len);
	}
	tcp_recved(tpcb, p->tot_len);

	/* As chained pbufs are not supported by lwip stack implementation
	 * we use PBUF_FLAG_PUSH flag as data packet end indicator */
	if (PBUF_FLAG_PUSH & (p->flags))   /* TODO: Add expected data length - low priority */
	{
      /* All data received. Close connection */
      receiveComplete(arg);
	}

	pbuf_free(p);
	ret_err = ERR_OK;
  }
  else if (es->state == ES_RECEIVED)
  {
	DBG_LOG(REST_MGR, "ES_RECEIVED");
	ret_err = ERR_OK;
  }
  else
  {
	/* Acknowledge data reception */
	tcp_recved(tpcb, p->tot_len);
	/* Free pbuf and do nothing */
	pbuf_free(p);
	ret_err = ERR_OK;
  }

  return ret_err;
}

static void tcp_client_send(struct tcp_pcb *tpcb, struct client_struct * es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;
  while ((wr_err == ERR_OK) && (es->p_tx != NULL) && (es->p_tx->len <= tcp_sndbuf(tpcb)))
  {
    ptr = es->p_tx;
    /* Enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, TCP_WRITE_FLAG_COPY); //TCP_WRITE_FLAG_MORE
    if (wr_err == ERR_OK)
    {
      es->p_tx = ptr->next;
      if(es->p_tx != NULL)
      {
        pbuf_ref(es->p_tx);
      }
      pbuf_free(ptr);
    }
    else if (wr_err == ERR_MEM)
    {
      es->p_tx = ptr;
    }
    else
    {
      /* Other problem? */
    }
  }
}

static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct client_struct * es = (struct client_struct *) arg;
  LWIP_UNUSED_ARG(len);

  if(es->p_tx != NULL)
  {
    tcp_client_send(tpcb, es);
  }
  else
  {
    DBG_LOG(REST_MGR, "Message was sent");

    /* Prepare buffer for receive */
    restApiMsgBufPtr_g = restApiMsgBuf_g;
  }
  return ERR_OK;
}

static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct client_struct *es = (struct client_struct *) arg;

  if (es != NULL)
  {
    if ((es->p_tx != NULL) && (es->state == ES_CLOSING))
    {
	  tcp_client_connection_close(tpcb, es);
#ifdef DEBUG
	  DBG_LOG(REST_MGR, "Connection closed: %d", connNum_g++);
#endif
	}
	//ret_err = ERR_OK;
  }
  else
  {
    tcp_abort(tpcb);
	ret_err = ERR_ABRT;
	DBG_LOG(REST_MGR, "Connection aborted");
  }
  return ret_err;
}
#endif
