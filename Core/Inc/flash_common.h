/********************************************************************************
  * @file    flash_common.h
  * @brief   System wide defines for SPI flash
  *
********************************************************************************/

#ifndef FLASH_CMN_H
#define FLASH_CMN_H

#define FLASH_SECTOR_SIZE    4096
#define FLASH_PAGE_SIZE      256

#define WRITE_BUFFER_SIZE  FLASH_PAGE_SIZE

#define SYS_CONFIG_SECTOR_START			0x000000
#define SYS_CONFIG_SECTOR_END			0x000FFF
#define NETWORK_CONFIG_SECTOR_START		0x001000
#define NETWORK_CONFIG_SECTOR_END		0x001FFF
#define FIRST_APP_SECTOR_START			0x002000
#define FIRST_APP_SECTOR_END			0x081FFF
#define SECOND_APP_SECTOR_START			0x082000
#define SECOND_APP_SECTOR_END			0x101FFF

#define GUI_CACHE_SECTOR_START 0x200000
#define GUI_CACHE_SECTOR_END 0x9FFFFF

#define SYS_CONFIGS_VALID_FLAG        0x90FA45F1

/*
   When new firmware is received by application, it is stored in predefined external flash slot.
   Application then saves version and FW length to external flash and reboots.
   After reboot, bootloader verifies whether new firmware is available and valid
   and copies it to internal flash if true. If not, bootloader proceed to load existing application.
*/

#pragma pack(1)

typedef enum
{
  FS_OK = 0,
  FS_NOT_OK
} fstatus_t;

typedef enum
{
  CRC_NO_ERROR,
  CRC_TABLE_CORRUPT, /* TODO: When it may happen? Is it possible? */
  CRC_ERROR_SLOT1,
  CRC_ERROR_SLOT2,
  CRC_ERROR_INTERNAL /* TODO: How it should be loaded with internal FW error? */
} crc_err_t;

typedef struct
{
  uint8_t 	vMajor;
  uint8_t 	vMinor;
  uint8_t 	vBuild;
} fwVersion_t;

typedef struct
{
  fwVersion_t	version;
  uint32_t      size;
  fstatus_t     available;
  fstatus_t     valid;
  char          timeStamp[15];
} bootAreaSettings_t;

typedef struct
{
  fwVersion_t   version;
  fstatus_t     useNextTime;
  fstatus_t     valid;
} newAppSettings_t;

typedef struct
{
  fwVersion_t   version;
  uint32_t 	    size;
  fstatus_t     valid;
  char          timeStamp[15];
} internalAppSettings_t;

typedef struct
{
  bootAreaSettings_t     bootArea[2];
  newAppSettings_t       newApp;
  internalAppSettings_t  internalApp;
  fwVersion_t            bootCodeVer;
  uint32_t    settingsValidationFlag;
} fw_settings_t;
#pragma pack()

#endif
