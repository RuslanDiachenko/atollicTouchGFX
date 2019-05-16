#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{
}
#ifndef SIMULATOR
#include "cmsis_os.h"
#include "main.h"

extern "C"
{
#include "switch_manager.h"
}

extern osMailQId uiMsgBox_g;

static osEvent evt;
static ui_state_t *mail;
static uint8_t hour, minute, hF, dayOfWeek, date, month, sunState, hideSun;
static uint16_t sunriseMin, sunsetMin, totalSunTime, sunStateDivider;

void Model::tick()
{
  evt = osMailGet(uiMsgBox_g, 0);
  if (evt.status == osEventMail)
  {
    mail = (ui_state_t *) evt.value.p;
    if (mail)
    {
      if (mail->msgType == DATE_TIME_CHANGED)
      {
		if (mail->dateTime.time.Hours >= 12)
		{
		  hF = 1;
		  hour = mail->dateTime.time.Hours - 12;
		}
		else
		{
		  hF = 0;
		  hour = mail->dateTime.time.Hours;
		}
		minute = mail->dateTime.time.Minutes;
		dayOfWeek = mail->dateTime.date.WeekDay;
		date = mail->dateTime.date.Date;
		month = mail->dateTime.date.Month;

		if (RV_SUCCESS == SW_GetSunriseSunsetMinutes(&sunriseMin, &sunsetMin))
		{
		  totalSunTime = sunsetMin - sunriseMin;
		  sunStateDivider = totalSunTime / 9;
		  sunState = ((mail->dateTime.time.Hours * 60 + mail->dateTime.time.Minutes) - sunriseMin) / sunStateDivider;
		  if ((mail->dateTime.time.Hours * 60 + mail->dateTime.time.Minutes) > sunsetMin ||
		  		(mail->dateTime.time.Hours * 60 + mail->dateTime.time.Minutes) < sunriseMin)
		  {
		  	hideSun = 1;
		  }
		  else
		  {
		  	hideSun = 0;
		  }
		}
		else
		{
		  sunState = 0;
		  hideSun = 1;
		}
		modelListener->notifySunStateChanged(hour, minute, hF, dayOfWeek, date, month, sunState, hideSun);
      }
      else if(mail->msgType == SLEEP_AFTER_TIMER)
      {
        modelListener->hideAllWidgets();
      }
    }
    osMailFree(uiMsgBox_g, mail);
  }
}
#else
void Model::tick()
{
    
}
#endif
