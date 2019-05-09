#ifndef SDRAM_H
#define SDRAM_H

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdint.h>

#define SDRAM_TIMEOUT		0xFFFF
void SDRAM_Init(SDRAM_HandleTypeDef *hsdram);

#endif /* SDRAM_H */
