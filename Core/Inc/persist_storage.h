#ifndef PRESIST_STORAGE_H
#define PRESIST_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "flash_common.h"

#define PS_STACK_SIZE   512

#define FLASH_BEGIN_ADDR		0x0000
#define FLASH_END_ADDR			0xffffff
#define FLASH_BLOCK_SIZE_4K		0x0fff
#define FLASH_BLOCK_SIZE_32K	0x7fff
#define FLASH_BLOCK_SIZE_64K	0xffff

#define FLASH_BLOCK_ERASE_4K 	0x20
#define FLASH_BLOCK_ERASE_32K	0x52
#define FLASH_BLOCK_ERASE_64K	0xd8

#define PS_MGR "PRES"
#define PS_DATA_LEN	FLASH_PAGE_SIZE

typedef enum
{
  FLASH_READY,
  FLASH_READING,
  FLASH_WRITING,
  FLASH_ERASING,
  FLASH_WRITE_WAIT
} flash_state_t;

typedef struct
{
  uint32_t addr;
  uint8_t data[PS_DATA_LEN];
  uint32_t len;
} ps_queue_t;


RV_t PS_Write(uint32_t addr, const uint8_t *data, uint32_t len);
RV_t PS_Read(uint32_t addr, uint8_t *data, uint32_t len);
void PS_FlashGetState(uint8_t *state);
RV_t PS_Init(void);

#ifdef PS_ADDITIONAL
RV_t PS_GetManAndDevID(uint8_t *man, uint16_t *devID);
#endif /* PS_ADDITIONAL */

#ifdef __cplusplus
}
#endif

#endif /* PRESIST_STORAGE_H */
