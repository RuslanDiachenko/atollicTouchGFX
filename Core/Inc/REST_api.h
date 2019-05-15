/*******************************************************************************
* @file  REST_api.h
* @brief Sage Glass REST api header file
*
* This file provides firmware functions prototypes 
* to send and receive REST api requests
*
*******************************************************************************/
#ifndef __REST_API_H
#define __REST_API_H

#include <lwip.h>
#include "lwip/def.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"
#include "lwip/sys.h"

#define REST_API_MAX_IP_LEN   16 /* IPv4 shouldn't require more memory */
#define REST_API_MAX_URI_LEN  32 /* Maybe need to extend in future */

/* If option is enabled output requests will be allow.
 * Currently we don't use output request */
#define REST_OUT_RQST_ENABLE 0

typedef enum
{
  REST_API_OK,
  REST_API_FAIL,
  REST_API_NULL_PTR,
  REST_API_NO_MEM,
  REST_API_TIMEOUT,
  REST_API_NO_CONN
} restApiStatus_t;

#if REST_OUT_RQST_ENABLE
typedef enum
{
  REST_API_OUT_HTTP_POST,
  REST_API_OUT_HTTP_PUT,
  REST_API_OUT_HTTP_GET,
  REST_API_IN_HTTP_POST,
  REST_API_IN_HTTP_PUT,
  REST_API_IN_HTTP_GET,
  /* Add more request types here */

  REST_API_RQST_LAST
} restApiRqstType_t;

typedef struct
{
  char subIP[REST_API_MAX_IP_LEN];
  char subUrl[REST_API_MAX_URI_LEN];
  uint8_t subPeriod;
  uint16_t subOnChange;
  uint8_t curTime;
} subscriberData_t;

typedef restApiStatus_t (*restApiResp_cb_t)(uint16_t code, char *header, uint16_t headerLen, uint8_t *data, uint16_t dataLen);
#endif

restApiStatus_t RM_InitRESTApi(void);
#if REST_OUT_RQST_ENABLE
restApiStatus_t RM_PutMsgToRESTApiQueue(restApiRqstType_t type, char *destIP, uint16_t destPort,
		                             char *uri, char *data, uint16_t dataLen, restApiResp_cb_t rest_cb);
restApiStatus_t RM_subscribeToDataOrEvent(char *jsonData, uint16_t jsonDataLen);
#endif

/* GET requests */
restApiStatus_t RM_SetupSwitchDataCommChannel(struct tcp_pcb *pcb);
restApiStatus_t RM_GenerateSwitchDataAndSend(uint8_t keyPresType, uint8_t uniqueID, uint8_t level, uint8_t msgType);
restApiStatus_t RM_GenerateZoneConfigJson(char **json);
restApiStatus_t RM_GenerateSceneConfigJson(char **json);
restApiStatus_t RM_GenerateSystemConfigJson(char **json);
restApiStatus_t RM_GenerateInfoJson(char **json);
restApiStatus_t RM_GenerateErrorStatusJson(char **json);
restApiStatus_t RM_GenerateIPSettingsJson(char **json);
restApiStatus_t RM_GenerateSerialNumberJson(char **json);
restApiStatus_t RM_GenerateMacAddressJson(char **json);
restApiStatus_t RM_GenerateStoredApplicationsJson(char **json);
restApiStatus_t RM_GenerateBootErrorJson(char **json);
restApiStatus_t RM_GenerateMemoryValueJson(char **json, uint32_t addr, uint8_t memType);
/* PUT requests */
restApiStatus_t RM_ParseIPSettingsJsonAndProcess(char *json);
restApiStatus_t RM_ParseSerialNumberJsonAndProcess(char *json);
restApiStatus_t RM_ParseMacAddressJsonAndProcess(char *json);
restApiStatus_t RM_ParseBootErrorJsonAndProcess(char *json);
restApiStatus_t RM_ParseMemoryChangeJsonAndProcess(char *json, uint8_t memType);
restApiStatus_t RM_ParseClockJsonAndProcess(char *json);
restApiStatus_t RM_ParseZoneConfigJsonAndProcess(char *json);
restApiStatus_t RM_ParseSceneConfigJsonAndProcess(char *json);
restApiStatus_t RM_ParseSystemConfigJsonAndProcess(char *json);
restApiStatus_t RM_ParseZoneStatusJsonAndProcess(char *json);
restApiStatus_t RM_ParseSceneStatusJsonAndProcess(char *json);
/* POST requests */
restApiStatus_t RM_SetupFirmwareDownloadCommChannel(struct tcp_pcb *pcb, char *json);
restApiStatus_t RM_ParceRebootJsonAndProcess(struct tcp_pcb *pcb, char *json);
restApiStatus_t RM_ResetProcess(struct tcp_pcb *pcb);
restApiStatus_t RM_ExtFlashInitProcess(void);
#endif /*__REST_API_H */
