#include "CustomHAL.hpp"
#include "persist_storage.h"
#include "flash_common.h"
#include "main.h"

// This function is called whenever a bitmap is cached. Must copy a number of bytes from the (non-memory-mapped) source
// to the cache.
bool CustomHAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
    DBG_LOG("    ", "Block copy called. dest: 0x%08X, src: 0x%08X, num: 0x%08X.", (uint32_t)dest, (uint32_t)src, (uint32_t)numBytes);
  // If requested data is located within the memory block we have assigned for ExtFlashSection,
  // provide an implementation for data copying.
  if (((uint32_t)src >= 0x21000000) && ((uint32_t)src < 0x21800000))
  {
    uint32_t dataOffset = ((uint32_t)src - 0x21000000) + GUI_CACHE_SECTOR_START;
    // In this example we assume graphics is placed in SD card, and that we have an appropriate function
    // for copying data from there.
    // sdcard_read(dest, dataOffset, numBytes);
    bool status = (PS_Read(dataOffset, (uint8_t *)dest, numBytes) == RV_SUCCESS);
    DBG_LOG("    ", "Block copy status: %s.", status ? "success" : "fail");
    return status;
  }
  else
  {
    // For all other addresses, just use the default implementation.
    // This is important, as blockCopy is also used for other things in the core framework.
    return STM32F4HAL::blockCopy(dest, src, numBytes);
  }
}
