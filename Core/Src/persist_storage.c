#include "persist_storage.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;

#define FLASH_CMD_WRITE_EN       	0x06
#define FLASH_CMD_WRITE_DIS       	0x04
#define FLASH_CMD_READ_STAT1     	0x05
#define FLASH_CMD_READ_STAT2     	0x35
#define FLASH_CMD_READ_DATA      	0x03
#define FLASH_CMD_PAGE_WRITE     	0x02
#define FLASH_CMD_CHIP_ERASE		0x60
#define FLASH_CMD_GET_MAN_AND_ID	0x9f

#define FLASH_CLOCK_DUMMY			0x00

#define FLASH_SELECT  				HAL_GPIO_WritePin(FLASH_IC_CS_GPIO_Port, FLASH_IC_CS_Pin, GPIO_PIN_RESET);
#define FLASH_DESELECT				HAL_GPIO_WritePin(FLASH_IC_CS_GPIO_Port, FLASH_IC_CS_Pin, GPIO_PIN_SET);

#define FLASH_SPI_INTERFACE			&hspi1

static osMutexId psMutexHandle;

static osMailQDef(psMsgBox_g, 5, ps_queue_t);
static osMailQId  psMsgBox_g;

static flash_state_t flashState;

static void flashSetState(uint8_t state)
{
  osMutexWait(psMutexHandle, osWaitForever);
  flashState = state;
  osMutexRelease(psMutexHandle);
}

void PS_FlashGetState(uint8_t *state)
{
  if (!state)
  {
	DBG_ERR(PS_MGR, "Null pointer received");
    return;
  }

  osMutexWait(psMutexHandle, osWaitForever);
  *state = flashState;
  osMutexRelease(psMutexHandle);
}

/* @brief	Put message to persist storage queue
 * @param	addr - address of first byte
 * 			data - pointer to data we want to program
 * 			len - number of bytes
 * @return	RV_SUCCESS in case of success
 * 			RV_FAILURE in case of failure */
static RV_t putWriteMsg(uint32_t addr, const uint8_t *data, uint32_t len)
{
	osStatus status = osOK;
	ps_queue_t *mail = 0;

	if (addr > FLASH_END_ADDR)
	{
		DBG_ERR(PS_MGR, "Received too big address value");
		return RV_FAILURE;
	}
	if(!data)
	{
		DBG_ERR(PS_MGR, "Received null pointer");
	}
	if(len > sizeof(mail->data))
	{
		DBG_ERR(PS_MGR, "Failed to write to flash. Up to %u bytes can be written at once",
				sizeof(mail->data));
		return RV_FAILURE;
	}

	mail = (ps_queue_t *) osMailAlloc(psMsgBox_g, 100);
	if(0 == mail)
	{
	    DBG_ERR(PS_MGR, "Failed to allocate memory for PS mail");
	    return RV_FAILURE;
	}

	mail->addr = addr;
	memcpy(mail->data, data, sizeof(mail->data));
	mail->len = len;

	status = osMailPut(psMsgBox_g, mail);
	if (osOK != status)
	{
	  DBG_ERR(PS_MGR, "Failed to add mail to PS mail queue. Error:%u", status);
	  return RV_FAILURE;
	}

	return RV_SUCCESS;
}

/* @brief	Programming byte, few bytes or page
 * @param	addr - address of first byte
 * 			data - pointer to data we want to program
 * 			len - number of bytes
 * @return	RV_SUCCESS in case of success
 * 			RV_NULLPTR in case of data null pointer
 * 			RV_FAILURE in case of failure
 * @note: 	Programming 1 page requires 0.7 ms delay.
 * 			You can program no more than 1 page at a time.
 * 			If the data goes beyond the page, it will
 * 			start recording at the beginning of the page */
static RV_t writeToFlash(uint32_t addr, uint8_t *data, uint16_t len)
{
	if (addr > FLASH_END_ADDR)
	{
		DBG_ERR(PS_MGR, "Too big address value");
		return RV_FAILURE;
	}
	if (len > FLASH_PAGE_SIZE)
	{
		DBG_ERR(PS_MGR, "Too big length value");
		return RV_FAILURE;
	}
	if (data == NULL)
	{
		DBG_ERR(PS_MGR, "Null pointer received");
		return RV_NULLPTR;
	}

	uint8_t sendCom[4] = {0};
	sendCom[0] = FLASH_CMD_PAGE_WRITE;
	sendCom[1] = (addr >> 16) & 0xFF;
	sendCom[2] = (addr >> 8) & 0xFF;
	sendCom[3] = addr & 0xFF;

	FLASH_SELECT;
	if (HAL_SPI_Transmit(FLASH_SPI_INTERFACE, sendCom, 4, 100))
	{
		FLASH_DESELECT;
		DBG_ERR(PS_MGR, "Problem with data sending");
		return RV_FAILURE;
	}
	if (HAL_SPI_Transmit(FLASH_SPI_INTERFACE, data, len, 1000))
	{
		FLASH_DESELECT;
		DBG_ERR(PS_MGR, "Problem with data sending");
		return RV_FAILURE;
	}
	FLASH_DESELECT;

	return RV_SUCCESS;
}

/* @brief	Read data from Flash
 * @param	addr - address of first byte
 * 			data - pointer to store data from Flash
 * 			size - number of bytes
 * @return 	RV_SUCCESS in case of success
 * 			RV_FAILURE in case of failure
 * @note	If address of byte will be bigger than 0x1FFFFF
 * 			Flash returns data from the beginning of memory*/
static RV_t readFromFlash(uint32_t addr, uint8_t *data, uint32_t len)
{
	RV_t state = RV_SUCCESS;

	if (addr > FLASH_END_ADDR)
	{
		DBG_ERR(PS_MGR, "Too big address value");
		return RV_FAILURE;
	}

	osMutexWait(psMutexHandle, osWaitForever);

	do
	{
		if (FLASH_READY != flashState)
		{
			DBG_ERR(PS_MGR, "Failed to read data from flash. Flash is busy");
		    state = RV_NOT_READY;
		    break;
		}

		flashState = FLASH_READING;

		uint8_t sendCom[FLASH_PAGE_SIZE] = {0};

		FLASH_SELECT;

		sendCom[0] = FLASH_CMD_READ_DATA;
		sendCom[1] = ((addr) >> 16) & 0xFF;
		sendCom[2] = ((addr) >> 8) & 0xFF;
		sendCom[3] = (addr) & 0xFF;

		if( HAL_SPI_Transmit(FLASH_SPI_INTERFACE, sendCom, 4, 100))
		{
			DBG_ERR(PS_MGR, "Problem with data sending");
			state = RV_FAILURE;
			break;
		}

		for (uint32_t i = 0; i < len;)
		{
			uint16_t receiveLength = 60000;
			if ((len - i) < receiveLength)
			{
					receiveLength = (len - i);
			}

			if (HAL_SPI_Receive(FLASH_SPI_INTERFACE, data + i, receiveLength, 1000))
			{
				DBG_ERR(PS_MGR, "Problem with data receiving");
				state = RV_FAILURE;
				break;
			}

			i += receiveLength;
		}

		if (state == RV_FAILURE)
		{
			break;
		}

		flashState = FLASH_READY;
	} while(0);
	FLASH_DESELECT;

	osMutexRelease(psMutexHandle);

	return state;
}

/* @brief	Send Erase Block command to Flash
 * @param	eraseBlockType - type of block we need
 * 			to erase (4/32/64 kB)
 * 			addr - address of block
 * @return	RV_SUCCESS in case of success
 * 			RV_FAILURE in case of failure
 * @note: 	erasing 4 kB block requires 70 ms delay,
 * 			32 kB block - 300 ms delay, 64 kB block - 600 ms delay */
static RV_t eraseBlock(uint8_t eraseBlockType, uint32_t addr)
{
	uint8_t sendCom[4] = {0};
	sendCom[0] = eraseBlockType;
	sendCom[1] = (addr >> 16) & 0xFF;
	sendCom[2] = (addr >> 8) & 0xFF;
	sendCom[3] = addr & 0xFF;

	FLASH_SELECT;
	if (HAL_SPI_Transmit(FLASH_SPI_INTERFACE, sendCom, 4, 100))
	{
		FLASH_DESELECT;
		DBG_ERR(PS_MGR, "Problem with data sending");
		return RV_FAILURE;
	}
	FLASH_DESELECT;

	return RV_SUCCESS;
}

/* @brief	Send Write Enable command to Flash
 * @param	None
 * @return	RV_SUCCESS in case of success
 * 			RV_FAILURE in case of failure */
static RV_t writeEnable(void)
{
	uint8_t sendCom[2];
	sendCom[0] = FLASH_CMD_WRITE_EN;

	FLASH_SELECT;
	if (HAL_SPI_Transmit(FLASH_SPI_INTERFACE, sendCom, 1, 100))
	{
		FLASH_DESELECT;
		DBG_ERR(PS_MGR, "Problem with data sending");
		return RV_FAILURE;
	}
	FLASH_DESELECT;

	return RV_SUCCESS;
}

/* @brief	Read Status Register Busy bit
 * @param	None
 * @return	1 - if flash is busy, 0 if flash is ready
 * 			for new command */
static uint8_t flashIsBusy(void)
{
	uint8_t status[2] = {0};
	uint8_t data[2] = {FLASH_CMD_READ_STAT1, 0};

	FLASH_SELECT;
	HAL_SPI_TransmitReceive(FLASH_SPI_INTERFACE, data, status, 2, 100);
	FLASH_DESELECT;

	return (status[1] & 0x01);
}

void PersistStorageTask(void const *argument)
{
  osEvent evt;
  uint8_t state = 0;
  ps_queue_t *mail;
  while (1)
  {
	evt = osMailGet(psMsgBox_g, osWaitForever);
	if (evt.status == osEventMail)
	{
	  mail = (ps_queue_t *) evt.value.p;

	  if (0 != mail)
	  {
		if (!(mail->addr % FLASH_SECTOR_SIZE))
		{
		  flashSetState(FLASH_WRITING);
		}
		else
		{
		  flashSetState(FLASH_ERASING);
		}

		do
		{
		  PS_FlashGetState(&state);

		  switch (state)
		  {
		    case FLASH_WRITING:
		      writeEnable();
		      eraseBlock(FLASH_BLOCK_ERASE_4K, mail->addr);

		      flashSetState(FLASH_ERASING);
		      break;

		    case FLASH_ERASING:
		      if (!flashIsBusy())
		      {
			    writeEnable();
			    writeToFlash(mail->addr, mail->data, mail->len);

			    flashSetState(FLASH_WRITE_WAIT);
		      }
		      break;

		    case FLASH_WRITE_WAIT:
		      if(!flashIsBusy())
		      {
		        flashSetState(FLASH_READY);
		      }
		      break;

		    default:
		      break;
		  }

		  osDelay(5);

		  PS_FlashGetState(&state);

		} while (state != FLASH_READY);

		osMailFree(psMsgBox_g, mail);
	  }
	}
  }
}

/* @brief	Initialize external Flash
 * @param	None
 * @return	RV_SUCCESS */
RV_t PS_Init(void)
{
	psMsgBox_g = osMailCreate(osMailQ(psMsgBox_g), NULL);
	if (0 == psMsgBox_g)
	{
		DBG_ERR(PS_MGR, "Failed to create PS mail queue");
		return RV_FAILURE;
	}

	osMutexDef(PSMutex);
	psMutexHandle = osMutexCreate(osMutex(PSMutex));
	if (NULL == psMutexHandle)
	{
		DBG_ERR(PS_MGR, "Failed to create PS mutex");
		return RV_FAILURE;
	}

	FLASH_SELECT;
	FLASH_DESELECT;

	return RV_SUCCESS;
}

RV_t PS_Read(uint32_t addr, uint8_t *data, uint32_t len)
{
  RV_t rv = RV_SUCCESS;
  uint8_t i = 1;

  while (RV_NOT_READY == (rv = readFromFlash(addr, data, len)))
  {
    /* allow current flash operation to finish */
    osDelay(300);

    if (3 == i)
    {
      break;
    }

    i++;
  }

  return rv;
}

RV_t PS_Write(uint32_t addr, const uint8_t *data, uint32_t len)
{

	RV_t rv = RV_FAILURE;
	uint32_t numOfChunks = len / FLASH_PAGE_SIZE;
	if (len % FLASH_PAGE_SIZE)
	{
		numOfChunks++;
	}

	for (uint32_t i = 0; i < numOfChunks; i++)
	{
		rv = putWriteMsg(addr + (i * FLASH_PAGE_SIZE), &data[i * FLASH_PAGE_SIZE],
				(len < FLASH_PAGE_SIZE ? len : FLASH_PAGE_SIZE));
		if (RV_SUCCESS != rv)
		{
			return rv;
		}
	}

	return RV_SUCCESS;
}


#ifdef PS_ADDITIONAL

/* @brief	Get manufacturer number and device ID
 * @param	man - pointer to store manufacturer number
 * 			devID - pointer to store device ID
 * return	RV_SUCESS in case of success
 * 			RV_FAILURE in case of failure */

RV_t PS_GetManAndDevID(uint8_t *man, uint16_t *devID)
{
  uint8_t sendCom[4] = {0};
  sendCom[0] = FLASH_CMD_GET_MAN_AND_ID;
  uint8_t recBuf[4] = {0};

  FLASH_SELECT;
  if (HAL_SPI_TransmitReceive(FLASH_SPI_INTERFACE, sendCom, recBuf, 4, 100))
  {
	  FLASH_DESELECT;
	  DBG_ERR(PS_MGR, "Problem with data sending/receiving");
	  return RV_FAILURE;
  }
  FLASH_DESELECT;
  *man = recBuf[1];
  *devID = recBuf[2] << 8 | recBuf[3];

  return RV_SUCCESS;
}


/* @brief	Send Write Disable command to Flash
 * @param	None
 * @return	RV_SUCCESS in case of success
 * 			RV_FAILURE in case of failure */

static RV_t writeDisable(void)
{
	uint8_t sendCom[2];
	sendCom[0] = FLASH_CMD_WRITE_DIS;
	FLASH_SELECT;
	if (HAL_SPI_Transmit(FLASH_SPI_INTERFACE, sendCom, 1, 100))
	{
		FLASH_DESELECT;
		DBG_ERR(PS_MGR, "Problem with data sending");
		return RV_FAILURE;
	}
	FLASH_DESELECT;

	return RV_SUCCESS;
}

/* @brief	Send Chip Erase command to Flash
 * @param	None
 * @return	RV_SUCCESS in case of success
 * 			RV_FAILURE in case of failure
 * @note	It has been experimentally determined
 * 			that the device needs a delay to erase
 * 			all of the memory for about 1.5 seconds  */

static RV_t chipErase(void)
{
	uint8_t sendCom[2] = {0};
	sendCom[0] = FLASH_CMD_CHIP_ERASE;
	FLASH_SELECT;
	if (HAL_SPI_Transmit(FLASH_SPI_INTERFACE, sendCom, 1, 100))
	{
		FLASH_DESELECT;
		DBG_ERR(PS_MGR, "Problem with data sending");
		return RV_FAILURE;
	}
	FLASH_DESELECT;

	return RV_SUCCESS;
}

#endif /* PS_ADDITIONAL */
