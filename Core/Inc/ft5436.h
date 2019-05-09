/*
 * ft5436.h
 *
 *  Created on: Mar 11, 2019
 *      Author: Ivan_Kubara
 */
#ifndef FT5436_H_
#define FT5436_H_

#include "main.h"
#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "math.h"

typedef struct
{
	uint8_t pressed;
	uint16_t x;
	uint16_t y;
	uint8_t z;
	//uint8_t gestureId;
} ctp_state_tp;


/**
* @
* @note Function initializes Touch panel
* @param none
* 	      Main tasks:
* 	        - Initialize Touch panel (send to 0x00 register 0b00000000)
* @retval RV_SUCCESS if success
*/

RV_t FT_Init(void);

/**
* @
* @note Function receives handles click from Touch panel
* @param struct with status, x, y coordinates, forse of click
* 		typedef struct
*		{
*		uint8_t pressed;
*		uint16_t x;
*		uint16_t y;
*		uint8_t z;
*		} ctp_state_tp;
*
* 	      Main tasks:
* 	        -  Receive data from TP
* 	        -  Return action, and coordinates to the struct with choosen inside function orientation mode
*
** @retval RV_SUCCESS if success
*/
RV_t FT_GetState(ctp_state_tp *t);

#endif /* FT5436_H_ */
