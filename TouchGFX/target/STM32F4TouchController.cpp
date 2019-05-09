#include <STM32F4TouchController.hpp>

/* USER CODE BEGIN BSP user includes */

/* USER CODE END BSP user includes */

extern "C"
{
#include "ft5436.h"
uint32_t LCD_GetXSize();
uint32_t LCD_GetYSize();
}

using namespace touchgfx;

void STM32F4TouchController::init()
{
   /* USER CODE BEGIN F4TouchController_init */

    /* Add code for touch controller Initialization*/
    FT_Init();

  /* USER CODE END  F4TouchController_init  */
}

bool STM32F4TouchController::sampleTouch(int32_t& x, int32_t& y)
{
  /* USER CODE BEGIN  F4TouchController_sampleTouch  */
    
	ctp_state_tp state;
	FT_GetState(&state);

    if (state.pressed  && (state.x < 480) && (state.x > 0)
               && (state.y < 272) && (state.y > 0))
    {
    	x = state.x;
    	y = state.y;
    	return true;
    }
    return false;

 /* USER CODE END F4TouchController_sampleTouch */    
}
