#ifndef __FW_UPDATE_H
#define __FW_UPDATE_H

#include "main.h"
#include "flash_common.h"
#include "persist_storage.h"

#define FW_UPD_CHUNK_SIZE   128
#define FW_UPD_TERMINATOR "END.END.END.END.END.END.END.END.END.END."
#define FW_UPD_TIMER_POLL_PERIOD_MS 50
#define FW_UPD_TIMEOUT_MS 2000

#define FW_UPDATE "FIRM"

#define APPLICATION_MEMORY_START  0x8008000
#define APPLICATION_MEMORY_END    0x8200000

RV_t FW_CheckAtStartup(void);
RV_t FW_StartUpdate(uint32_t majorVer, uint32_t minorVer, uint32_t buildVer,
		uint32_t bytesLen, char *timeStamp, uint32_t slotNum);
RV_t FW_SaveDataChunk(uint8_t *fwData, uint16_t fwDataLen);
RV_t FW_FinalizeUpdate(void);
RV_t FW_SaveUpdateSettings(void);
RV_t FW_GetSettingsPtr(fw_settings_t **settings);
RV_t FW_UpdateCancel(void);

#endif /* __FW_UPDATE_H */
