#include <string.h>
#include "stm32f4xx_hal.h"
#include "fw_update.h"
#include "REST_api.h"
#include "pcs_json.h"
#include "crc32.h"

/* Set to "1" to check firmware instead writing to external flash */
#define TEST_VALIDATE_FIRMWARE 0

typedef enum
{
  UPD_STATUS_OFF,
  UPD_STATUS_IN_PROGRESS
} upd_status_t;

static struct
{
  upd_status_t upd_status;
  /* Total length of new FW length and index of current chunk */
  uint32_t fwTotalLen;
  uint32_t fwIdx;
  /* Slot start address */
  uint32_t slotAddr;
  uint8_t  slot;
} fwUpdCtx_g;

/* Current firmware settings retrieved from external flash */
static fw_settings_t actualSettings_g;

RV_t FW_UpdateCancel(void)
{
  /* Set FW update runtime data to default state */
  memset(&fwUpdCtx_g, 0x00, sizeof(fwUpdCtx_g));

  return RV_SUCCESS;
}

RV_t FW_GetSettingsPtr(fw_settings_t **settings)
{
  *settings = &actualSettings_g;

  return RV_SUCCESS;
}

RV_t FW_SaveUpdateSettings(void)
{
  if (RV_SUCCESS != PS_Write(SYS_CONFIG_SECTOR_START, (const uint8_t *) &actualSettings_g, sizeof(actualSettings_g)))
  {
    DBG_ERR(FW_UPDATE, "Failed to save FW configuration data to persistent storage");

    return RV_FAILURE;
  }

  return RV_SUCCESS;
}

RV_t FW_CheckAtStartup(void)
{
  /* Read firmware settings from external flash */
  if (RV_SUCCESS != PS_Read(SYS_CONFIG_SECTOR_START, (uint8_t *)(&actualSettings_g), sizeof(actualSettings_g)))
  {
    DBG_ERR(FW_UPDATE, "Failed to read firmware settings from flash memory");
    return RV_FAILURE;
  }

  actualSettings_g.internalApp.version.vMajor = FW_VERSION_MAJOR;
  actualSettings_g.internalApp.version.vMinor = FW_VERSION_MINOR;
  actualSettings_g.internalApp.version.vBuild = FW_VERSION_BUILD;

  /* Check firmware settings */
  if ((actualSettings_g.newApp.valid) || (actualSettings_g.internalApp.valid))
  {
    /* Supposed some crc32 checksum error - light on RED LED */
	/* TODO: add led */
	//HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
	DBG_LOG(FW_UPDATE, "New app valid flag = %d, Internal app valid flag = %d",
			actualSettings_g.newApp.valid, actualSettings_g.internalApp.valid);
  }
  else
  {
	  /* TODO: add led */
	//HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
  }

  return RV_SUCCESS;
}

RV_t FW_StartUpdate(uint32_t majorVer, uint32_t minorVer, uint32_t buildVer,
		uint32_t bytesLen, char *timeStamp, uint32_t slotNum)
{
  if ((1 != slotNum) && (2 != slotNum))
  {
    DBG_ERR(FW_UPDATE, "Not correct slot number. Can't start firmware update");
    return RV_FAILURE;
  }

  actualSettings_g.bootArea[slotNum - 1].version.vMajor = majorVer;
  actualSettings_g.bootArea[slotNum - 1].version.vMinor = minorVer;
  actualSettings_g.bootArea[slotNum - 1].version.vBuild = buildVer;
  actualSettings_g.bootArea[slotNum - 1].size = bytesLen;
  strncpy(actualSettings_g.bootArea[slotNum - 1].timeStamp, timeStamp,
			sizeof(actualSettings_g.bootArea[slotNum - 1].timeStamp));
  actualSettings_g.newApp.version.vMajor = majorVer;
  actualSettings_g.newApp.version.vMinor = minorVer;
  actualSettings_g.newApp.version.vBuild = buildVer;

  /* Initialize firmware update context */
  fwUpdCtx_g.upd_status = UPD_STATUS_IN_PROGRESS;
  fwUpdCtx_g.fwIdx = 0;
  fwUpdCtx_g.fwTotalLen = 0;
  fwUpdCtx_g.slotAddr = (1 == slotNum) ? FIRST_APP_SECTOR_START : SECOND_APP_SECTOR_START;
  fwUpdCtx_g.slot = (uint8_t) slotNum;

  return RV_SUCCESS;
}

#if TEST_VALIDATE_FIRMWARE
uint8_t testBuff[128];
#endif

RV_t FW_SaveDataChunk(uint8_t *fwData, uint16_t fwDataLen)
{
  uint32_t offset = 0;

  if ((NULL == fwData) || (0 == fwDataLen))
  {
    DBG_ERR(FW_UPDATE, "Not correct input parameters in FW_SaveDataChunk()");
    return RV_FAILURE;
  }

  if (fwUpdCtx_g.upd_status != UPD_STATUS_IN_PROGRESS)
  {
    DBG_ERR(FW_UPDATE, "Update not in progress. Interrupt fake request");
    return RV_FAILURE;
  }

  DBG_LOG(FW_UPDATE, "Saving FW chunk #%lu to flash...", fwUpdCtx_g.fwIdx + 1);

  offset = fwUpdCtx_g.fwIdx * FW_UPD_CHUNK_SIZE;

  fwUpdCtx_g.fwIdx++;

#if !(TEST_VALIDATE_FIRMWARE)
  if (RV_SUCCESS != PS_Write(fwUpdCtx_g.slotAddr + offset, fwData, fwDataLen))
  {
    DBG_ERR(FW_UPDATE, "Failed to save firmware chunk to flash");

    FW_UpdateCancel();

    return RV_FAILURE;
  }
#else
  if (RV_SUCCESS != PS_Read(fwUpdCtx_g.slotAddr + offset, testBuff, fwDataLen))
  {
    DBG_ERR(FW_UPDATE, "Failed to save firmware chunk to flash");

    FW_UpdateCancel();

    return RV_FAILURE;
  }
  if (0 == memcmp(testBuff, fwData, sizeof(testBuff)))
  {
	DBG_LOG(FW_UPDATE, "OK");
  }
  else
  {
	DBG_ERR(FW_UPDATE, "NOT OK");
  }
#endif

  fwUpdCtx_g.fwTotalLen += FW_UPD_CHUNK_SIZE;

  return RV_SUCCESS;
}


RV_t FW_FinalizeUpdate(void)
{
  uint8_t data[FW_UPD_CHUNK_SIZE] = {0};
  uint32_t crcValue = 0xFFFFFFFF; /* Initial crc value */
  uint32_t offset = 0;
  RV_t status = RV_SUCCESS;

  /* Check firmware CRC32 checksum */
  DBG_LOG(FW_UPDATE, "All chunks are retrieved. Calculating CRC...");

  for (uint32_t i = 0; i < (fwUpdCtx_g.fwIdx); i++)
  {
    offset = i * FW_UPD_CHUNK_SIZE;

    /* Read firmware settings from external flash */
	if (RV_SUCCESS != PS_Read(fwUpdCtx_g.slotAddr + offset, data, FW_UPD_CHUNK_SIZE))
	{
	  DBG_ERR(FW_UPDATE, "Failed to read firmware from flash memory. Chunk #%lu", i + 1);
	  return RV_FAILURE;
	}
	crcValue = crc32_buffer(FW_UPD_CHUNK_SIZE, crcValue, data);
  }

  if (0 == crcValue)
  {
    DBG_LOG(FW_UPDATE, "CRC is OK. Applying new FW");
    actualSettings_g.bootArea[fwUpdCtx_g.slot - 1].available = FS_OK;
    actualSettings_g.bootArea[fwUpdCtx_g.slot - 1].valid = FS_OK;
    actualSettings_g.newApp.valid = FS_OK;
    actualSettings_g.newApp.useNextTime = FS_OK;
    status = RV_SUCCESS;
  }
  else
  {
    DBG_LOG(FW_UPDATE, "CRC is Not OK. Terminate update");
    /* TODO: Do we need to save invalid markers to flash? */
    actualSettings_g.bootArea[fwUpdCtx_g.slot - 1].available = FS_OK;
    actualSettings_g.bootArea[fwUpdCtx_g.slot - 1].valid = FS_NOT_OK;
    actualSettings_g.newApp.valid = FS_NOT_OK;
    actualSettings_g.newApp.useNextTime = FS_NOT_OK;
    status = RV_FAILURE;
  }

  if (RV_SUCCESS != PS_Write(SYS_CONFIG_SECTOR_START, (const uint8_t *) &actualSettings_g, sizeof(actualSettings_g)))
  {
    DBG_ERR(FW_UPDATE, "Failed to save FW configuration data to persistent storage");

    return RV_FAILURE;
  }

  return status;
}
