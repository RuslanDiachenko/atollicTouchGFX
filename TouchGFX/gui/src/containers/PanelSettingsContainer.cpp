#include <gui/containers/PanelSettingsContainer.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

#ifndef SIMULATOR
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h" 

extern TIM_HandleTypeDef htim4;
extern osTimerId sleepAfterTimerHandle;

sleep_after_state_t sleepAfterState_g = {1, 0, 60}; 
#endif

PanelSettingsContainer::PanelSettingsContainer():
  m_currentPressedButton(PressedButton::UrbanStyle)
{
  urbanStyleButton.forceState(true);
  startUrbanStyle();
  //urbanStyleButtonClicked();
}

void PanelSettingsContainer::initialize()
{
    PanelSettingsContainerBase::initialize();
}

void PanelSettingsContainer::CloseButtonClicked()
{
    m_pCloseContainerCallback->execute();
}

void PanelSettingsContainer::urbanStyleButtonClicked(void)
{
  m_currentPressedButton = PressedButton::UrbanStyle;
    
  urbanStyleButton.setTouchable(false);
   
  darkStyleButton.setTouchable(true);
  darkStyleButton.forceState(false);
  darkStyleButton.invalidate();
    
  lightStyleButton.setTouchable(true);
  lightStyleButton.forceState(false);
  lightStyleButton.invalidate();
  
  m_pSetStyleCallback->execute(2);
}

void PanelSettingsContainer::darkStyleButtonClicked(void)
{
  m_currentPressedButton = PressedButton::DarkStyle;
    
  darkStyleButton.setTouchable(false);
    
  urbanStyleButton.setTouchable(true);
  urbanStyleButton.forceState(false);
  urbanStyleButton.invalidate();
    
  lightStyleButton.setTouchable(true);
  lightStyleButton.forceState(false);
  lightStyleButton.invalidate();
  
  m_pSetStyleCallback->execute(1);
}

void PanelSettingsContainer::lightStyleButtonClicked(void)
{
  m_currentPressedButton = PressedButton::LightStyle;
    
  lightStyleButton.setTouchable(false);
    
  urbanStyleButton.setTouchable(true);
  urbanStyleButton.forceState(false);
  urbanStyleButton.invalidate();
    
  darkStyleButton.setTouchable(true);
  darkStyleButton.forceState(false);
  darkStyleButton.invalidate();
  
  m_pSetStyleCallback->execute(0);
}

void PanelSettingsContainer::startUrbanStyle(void)
{
  m_currentPressedButton = PressedButton::UrbanStyle;
    
  urbanStyleButton.setTouchable(false);
   
  darkStyleButton.setTouchable(true);
  darkStyleButton.forceState(false);
  darkStyleButton.invalidate();
    
  lightStyleButton.setTouchable(true);
  lightStyleButton.forceState(false);
  lightStyleButton.invalidate();
}

void PanelSettingsContainer::setStyle(int style)
{
  if (style == 2)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    panelBrightnessText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    sleepAfterText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    visualStyleText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    staticTextArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea2.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea3.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea4.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea5.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea6.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea7.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea8.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea9.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea10.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea11.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    
    panelBrightnessSlider.setBitmaps(Bitmap(BITMAP_SLIDERBASE2POINTS_ID), Bitmap(BITMAP_SLIDERBASE2POINTS_ID), Bitmap(BITMAP_SLIDERINDICATOR_ID));
    sleepAfterSlider.setBitmaps(Bitmap(BITMAP_SLIDERBASE4POINTS_ID), Bitmap(BITMAP_SLIDERBASE4POINTS_ID), Bitmap(BITMAP_SLIDERINDICATOR_ID));
    
    lightStyleButton.setBitmaps(Bitmap(BITMAP_WHITESTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_WHITESTYLEBUTTONPRESSED_ID));
    darkStyleButton.setBitmaps(Bitmap(BITMAP_DARKSTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_DARKSTYLEBUTTONPRESSED_ID));
    urbanStyleButton.setBitmaps(Bitmap(BITMAP_URBANSTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_URBANSTYLEBUTTONPRESSED_ID));
  }
  else if (style == 1)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    panelBrightnessText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    sleepAfterText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    visualStyleText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    staticTextArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea2.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea3.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea4.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea5.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea6.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea7.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea8.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea9.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea10.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    staticTextArea11.setColor(touchgfx::Color::getColorFrom24BitRGB(234, 234, 234));
    
    panelBrightnessSlider.setBitmaps(Bitmap(BITMAP_SLIDERBASE2POINTS_ID), Bitmap(BITMAP_SLIDERBASE2POINTS_ID), Bitmap(BITMAP_SLIDERINDICATOR_ID));
    sleepAfterSlider.setBitmaps(Bitmap(BITMAP_SLIDERBASE4POINTS_ID), Bitmap(BITMAP_SLIDERBASE4POINTS_ID), Bitmap(BITMAP_SLIDERINDICATOR_ID));
    
    lightStyleButton.setBitmaps(Bitmap(BITMAP_WHITESTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_WHITESTYLEBUTTONPRESSED_ID));
    darkStyleButton.setBitmaps(Bitmap(BITMAP_DARKSTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_DARKSTYLEBUTTONPRESSED_ID));
    urbanStyleButton.setBitmaps(Bitmap(BITMAP_URBANSTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_URBANSTYLEBUTTONPRESSED_ID));
  }
  else if (style == 0)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    panelBrightnessText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    sleepAfterText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    visualStyleText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    
    staticTextArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea2.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea3.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea4.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea5.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea6.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea7.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea8.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea9.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea10.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    staticTextArea11.setColor(touchgfx::Color::getColorFrom24BitRGB(21, 21, 21));
    
    panelBrightnessSlider.setBitmaps(Bitmap(BITMAP_SLIDERBASE2POINTSBLACK_ID), Bitmap(BITMAP_SLIDERBASE2POINTSBLACK_ID), Bitmap(BITMAP_SLIDERINDICATORBLACK_ID));
    sleepAfterSlider.setBitmaps(Bitmap(BITMAP_SLIDERBASE4POINTSBLACK_ID), Bitmap(BITMAP_SLIDERBASE4POINTSBLACK_ID), Bitmap(BITMAP_SLIDERINDICATORBLACK_ID));
    
    lightStyleButton.setBitmaps(Bitmap(BITMAP_WHITESTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_WHITESTYLEBUTTONPRESSEDBLACK_ID));
    darkStyleButton.setBitmaps(Bitmap(BITMAP_DARKSTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_DARKSTYLEBUTTONPRESSEDBLACK_ID));
    urbanStyleButton.setBitmaps(Bitmap(BITMAP_URBANSTYLEBUTTONNOTPRESSED_ID), Bitmap(BITMAP_URBANSTYLEBUTTONPRESSEDBLACK_ID));
  }
  
  containerNameText.invalidate();
  panelBrightnessText.invalidate();
  sleepAfterText.invalidate();
  visualStyleText.invalidate();
  
  staticTextArea1.invalidate();
  staticTextArea2.invalidate();
  staticTextArea3.invalidate();
  staticTextArea4.invalidate();
  staticTextArea5.invalidate();
  staticTextArea6.invalidate();
  staticTextArea7.invalidate();
  staticTextArea8.invalidate();
  staticTextArea9.invalidate();
  staticTextArea10.invalidate();
  staticTextArea11.invalidate();
  
  panelBrightnessSlider.invalidate();
  sleepAfterSlider.invalidate();
  
  if(m_currentPressedButton == PressedButton::UrbanStyle)
  {
    urbanStyleButton.forceState(true);
    darkStyleButton.forceState(false);
    lightStyleButton.forceState(false);
  }
  else if(m_currentPressedButton == PressedButton::DarkStyle)
  {
    urbanStyleButton.forceState(false);
    darkStyleButton.forceState(true);
    lightStyleButton.forceState(false);
  }
  else if(m_currentPressedButton == PressedButton::LightStyle)
  {
    urbanStyleButton.forceState(false);
    darkStyleButton.forceState(false);
    lightStyleButton.forceState(true);
  }
  
  urbanStyleButton.invalidate();
  darkStyleButton.invalidate();
  lightStyleButton.invalidate();
}

#ifdef hhh
static void changePWMOut(uint16_t pulseWidth)
{
  TIM_OC_InitTypeDef conf;

  conf.OCMode = TIM_OCMODE_PWM1;
  conf.Pulse = pulseWidth;
  conf.OCPolarity = TIM_OCPOLARITY_HIGH;
  conf.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_PWM_ConfigChannel(&htim4, &conf, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

void PanelSettingsContainer::panelBrightnessSliderCallback(int value)
{
  /*
  example of map function : 

  long map(long x, long in_min, long in_max, long out_min, long out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  } 
  */
  uint16_t newPWM = (value - 0) * (0 - 725) / (100 - 0) + 725;
  changePWMOut(newPWM);
}

void PanelSettingsContainer::sleepAfterSliderCallback(int value)
{
  uint16_t seconds = 0;
  sleepAfterState_g.infinity = 0;
  DBG_LOG("SLEP", "Value is - %d", value);
  if (value <= 33)
  {
    seconds = (value - 0) * (60 - 5) / (33 - 0) + 5;
  }
  else if (value > 33 && value <= 66)
  {
    seconds = (value - 34) * (300 - 60) / (64 - 34) + 60;
  }
  else if (value > 66 && value <= 98)
  {
    seconds = (value - 67) * (600 - 300) / (98 - 67) + 300;
  }
  else
  {
    seconds = 65535;
    sleepAfterState_g.infinity = 1;
  }
  sleepAfterState_g.duration = seconds;
  if (sleepAfterState_g.infinity)
    osTimerStop(sleepAfterTimerHandle);
  else 
    osTimerStart(sleepAfterTimerHandle, pdMS_TO_TICKS(seconds*1000));
  DBG_LOG("SLEP", "Seconds is - %d", seconds);
}
#else

void PanelSettingsContainer::panelBrightnessSliderCallback(int value)
{
  
}

void PanelSettingsContainer::sleepAfterSliderCallback(int value)
{
  
}
#endif
