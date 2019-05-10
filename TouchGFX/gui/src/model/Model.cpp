#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{
}
#ifdef hhh
#include "cmsis_os.h"
#include "main.h"

extern osMailQId sunMsgBox_g;

static osEvent evt;
ui_state_t *mail;

void Model::tick()
{
  static uint8_t prevHour = 0, prevMinute = 0;
//  static unsigned int counter = 0;
//  if (counter >= 20)
//  {
//    counter = 0;
    evt = osMailGet(sunMsgBox_g, 0);
    if (evt.status == osEventMail)
    {
      mail = (ui_state_t *) evt.value.p;
      if (mail)
      {
        if (mail->msgType == DATE_TIME_CHANGED)
        {
          if (prevHour != mail->dateTime.hour || prevMinute != mail->dateTime.minute)
          {
            modelListener->notifySunStateChanged(mail->dateTime.hour, mail->dateTime.minute, mail->dateTime.hF, mail->dateTime.dayOfWeek);
          }
        }
        else if(mail->msgType == SLEEP_AFTER_TIMER)
        {
          modelListener->hideAllWidgets();
        }
      }
      osMailFree(sunMsgBox_g, mail);
    }
//  }
//  counter++;
}
#else
void Model::tick()
{
    
}
#endif
