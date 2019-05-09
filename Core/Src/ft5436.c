
#include "ft5436.h"

extern I2C_HandleTypeDef hi2c1;

#define I2C_TOUCH_PANEL				&hi2c1 			/* Number of I2C in MCU */
#define TP_I2C_ADDRESS 				(0x38<<1) 		/* Address of touch*/
#define INI_VALUE 					0  				/* Value, which must be written to "Device mode" register to ini touch*/
#define X_PIXELS 					480
#define Y_PIXELS					272
#define OFFSET_X					0
#define OFFSET_y					0
#define ORIENTATION              	0  /* 0 Landscape 1 Portrait*/




RV_t FT_Init (void) /* Initialize touch */
{
	uint8_t buf[2];
	buf[0] = 0;
	buf[1] = 0;
	if (HAL_I2C_Master_Transmit(I2C_TOUCH_PANEL, TP_I2C_ADDRESS, buf, 2, 100) == HAL_OK)
    {
      return RV_SUCCESS;
    }
    return RV_FAILURE;
}

RV_t FT_GetState(ctp_state_tp *t) /*Receive and handle data from TP */
{
	uint8_t data[6] = {0};
	uint8_t com = 0x03;
	uint16_t xReal=0, yReal=0, xCalc = 0, yCalc=0, muller = 0x3fff;

	if (HAL_I2C_Master_Transmit(I2C_TOUCH_PANEL, TP_I2C_ADDRESS, &com, 1, 100 ))
	{
		return RV_FAILURE;
	}
	if (HAL_I2C_Master_Receive(I2C_TOUCH_PANEL, TP_I2C_ADDRESS, data, 6, 100))
	{
		return RV_FAILURE;
	}
	if ((data[0]>>6)==1)
	{
		t->pressed = 0;
	}
	else
	{
		t->pressed = 1;
	}
	xReal=(data[0]<<8|data[1])&muller;
	yReal=(data[2]<<8)|data[3];

	if(ORIENTATION)
	{
	xCalc=Y_PIXELS-yReal;
	yCalc=xReal;
	}
	else
	{
	xCalc=xReal;
	yCalc=yReal;
	}

	t->x=xCalc;
	t->y=yCalc;
	t->z=data[5];

	return RV_SUCCESS;
}
