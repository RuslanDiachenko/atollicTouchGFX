#include <STM32F4TouchController.hpp>

/* USER CODE BEGIN BSP user includes */

/* USER CODE END BSP user includes */

extern "C"
{
#include "ft5436.h"
uint32_t LCD_GetXSize();
uint32_t LCD_GetYSize();
}

extern sleep_after_state_t sleepAfterState_g;
extern osTimerId sleepAfterTimerHandle;

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
        HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);
        if (!sleepAfterState_g.infinity)
          osTimerStart(sleepAfterTimerHandle, pdMS_TO_TICKS(sleepAfterState_g.duration * 1000));
    	return true;
    }
    return false;

 /* USER CODE END F4TouchController_sampleTouch */    
}
