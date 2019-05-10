#include <gui/main_screen/MainView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

#ifndef SIMULATOR
#include "main.h"
extern sleep_after_state_t sleepAfterState_g;
#endif

MainView::MainView() :
    openAllZonesContainer(this, &MainView::OpenAllZonesContainerHandler),
    closeAllZonesContainer(this, &MainView::CloseAllZonesContainerHandler),
    backAllZonesContainer(this, &MainView::BackAllZonesContainerHandler),
    openAllScenesContainer(this, &MainView::OpenAllScenesContainerHandler),
    closeAllScenesContainer(this, &MainView::CloseAllScenesContainerHandler),
    backAllScenesContainer(this, &MainView::BackAllScenesContainerHandler),
    closeWindowSettingsContainer(this, &MainView::CloseWindowSettingsContainerHandler),
    closePanelSettingsContainer(this, &MainView::ClosePanelSettingsContainerHandler),
    setStyle(this, &MainView::SetStyleHandler)
{
    allZonesContainer.SetCloseContainerCallback(closeAllZonesContainer);
    allZonesContainer.SetBackContainerCallback(backAllZonesContainer);
    allScenesContainer.SetBackContainerCallback(backAllScenesContainer);
    allScenesContainer.SetCloseContainerCallback(closeAllScenesContainer);
    windowSettingsContainer.SetCloseContainerCallback(closeWindowSettingsContainer);
    windowSettingsContainer.SetOpenManualTintContainerCallback(openAllZonesContainer);
    windowSettingsContainer.SetOpenAllScenesContainerCallback(openAllScenesContainer);
    panelSettingsContainer.SetCloseContainerCallback(closePanelSettingsContainer);
    panelSettingsContainer.SetStyleCallback(setStyle);
}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}


void MainView::BackAllScenesContainerHandler(void)
{
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.moveTo(280, 10);
    windowSettingsContainer.startMoveAnimation(10, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    allScenesContainer.clearMoveAnimationEndedAction();
    allScenesContainer.startMoveAnimation(-260, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);
}

void MainView::CloseAllScenesContainerHandler(void)
{
    allScenesContainer.clearMoveAnimationEndedAction();
    allScenesContainer.startMoveAnimation(-260, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    backgroundBlur.clearFadeAnimationEndedAction();
    backgroundBlur.setFadeAnimationDelay(8);
    backgroundBlur.startFadeAnimation(0, 4, EasingEquations::linearEaseIn);
    
    windowSettingsButton.setTouchable(true);
}

void MainView::OpenAllScenesContainerHandler(void)
{
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.startMoveAnimation(280, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    allScenesContainer.clearMoveAnimationEndedAction();
    allScenesContainer.moveTo(-260, 10);
    allScenesContainer.startMoveAnimation(10, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);
}

void MainView::BackAllZonesContainerHandler(void)
{
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.moveTo(280, 10);
    windowSettingsContainer.startMoveAnimation(10, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    allZonesContainer.clearMoveAnimationEndedAction();
    allZonesContainer.startMoveAnimation(-260, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);
}

void MainView::CloseAllZonesContainerHandler(void)
{
    allZonesContainer.clearMoveAnimationEndedAction();
    allZonesContainer.startMoveAnimation(-260, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    backgroundBlur.clearFadeAnimationEndedAction();
    backgroundBlur.setFadeAnimationDelay(8);
    backgroundBlur.startFadeAnimation(0, 4, EasingEquations::linearEaseIn);
    
    windowSettingsButton.setTouchable(true);
}

void MainView::OpenAllZonesContainerHandler(void)
{
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.startMoveAnimation(280, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    allZonesContainer.clearMoveAnimationEndedAction();
    allZonesContainer.moveTo(-260, 10);
    allZonesContainer.startMoveAnimation(10, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);
}

void MainView::WindowSettingsButtonClicked()
{
#ifndef SIMULATOR
    if (!sleepAfterState_g.screenState)
    {
      sleepAfterState_g.screenState = 1;
      return;
    }
#endif
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.moveTo(-260, 10);

    backgroundBlur.clearFadeAnimationEndedAction();
    backgroundBlur.setFadeAnimationDelay(0);
    backgroundBlur.startFadeAnimation(255, 4, EasingEquations::linearEaseIn);
    
    windowSettingsButton.setTouchable(false);
    
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.startMoveAnimation(10, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);
}

void MainView::CloseWindowSettingsContainerHandler(void)
{
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.startMoveAnimation(-260, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    backgroundBlur.clearFadeAnimationEndedAction();
    backgroundBlur.setFadeAnimationDelay(8);
    backgroundBlur.startFadeAnimation(0, 4, EasingEquations::linearEaseIn);
    
    windowSettingsButton.setTouchable(true);
}

void MainView::PanelSettingsButtonClicked()
{
#ifndef SIMULATOR
    if (!sleepAfterState_g.screenState)
    {
      sleepAfterState_g.screenState = 1;
      return;
    }
#endif
    m_lastBackgroundBlurAlfa = backgroundBlur.getAlpha();
    m_lastWindowSettingsButtonTouchable = windowSettingsButton.isTouchable();

    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.startMoveAnimation(
        windowSettingsContainer.getX(), 490, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn
    );

    allScenesContainer.clearMoveAnimationEndedAction();
    allScenesContainer.startMoveAnimation(
        allScenesContainer.getX(), 490, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn
    );

    allZonesContainer.clearMoveAnimationEndedAction();
    allZonesContainer.startMoveAnimation(
        allZonesContainer.getX(), 490, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn
    );

    backgroundBlur.clearFadeAnimationEndedAction();
    backgroundBlur.setFadeAnimationDelay(0);
    backgroundBlur.startFadeAnimation(255, 4, EasingEquations::linearEaseIn);

    panelSettingsContainer.clearMoveAnimationEndedAction();
    panelSettingsContainer.startMoveAnimation(10, 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    panelSettingsButton.setVisible(false);
    windowSettingsButton.setTouchable(false);
}

void MainView::ClosePanelSettingsContainerHandler(void)
{
    windowSettingsContainer.clearMoveAnimationEndedAction();
    windowSettingsContainer.startMoveAnimation(
        windowSettingsContainer.getX(), 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn
    );

    allScenesContainer.clearMoveAnimationEndedAction();
    allScenesContainer.startMoveAnimation(
        allScenesContainer.getX(), 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn
    );

    allZonesContainer.clearMoveAnimationEndedAction();
    allZonesContainer.startMoveAnimation(
        allZonesContainer.getX(), 10, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn
    );

    backgroundBlur.clearFadeAnimationEndedAction();
    backgroundBlur.setFadeAnimationDelay(0);
    backgroundBlur.startFadeAnimation(m_lastBackgroundBlurAlfa, 4, EasingEquations::linearEaseIn);

    panelSettingsContainer.clearMoveAnimationEndedAction();
    panelSettingsContainer.startMoveAnimation(10, -410, 12, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);

    panelSettingsButton.setVisible(true);
    windowSettingsButton.setTouchable(m_lastWindowSettingsButtonTouchable);
}


void MainView::setSunState(int hour, int minute, int hF, int dow)
{
    static int16_t prevSunState = -1; 
    int16_t endX, endY, newSunState = 0;
    uint16_t totalMin = minute + hour*60;
    uint8_t hideSun = 0;
    if (hF)
    {
      totalMin += 360;
    }
    else
    {
      totalMin -= 360;
    }
    
    newSunState = totalMin / 96;
    
    if ((hour >= 8 && minute >= 40 && hF == 1) || (hour < 6 && hF == 0))
    {
      hideSun = 1;
    }
    
    switch (newSunState)
    {
        case 0:
            endX = 23;
            endY = 374;
            break;
        case 1:
            endX = 43;
            endY = 352;
            break;
        case 2:
            endX = 66;
            endY = 336;
            break;
        case 3:
            endX = 94;
            endY = 324;
            break;
        case 4:
            endX = 123;
            endY = 320;
            break;
        case 5:
            endX = 154;
            endY = 324;
            break;
        case 6:
            endX = 182;
            endY = 336;
            break;
        case 7:
            endX = 206;
            endY = 352;
            break;
        case 8:
            endX = 225;
            endY = 374;
            break;
    }

    Unicode::snprintf(clockNumBuffer1, CLOCKNUMBUFFER1_SIZE, "%d", hour);
    Unicode::snprintf(clockNumBuffer2, CLOCKNUMBUFFER2_SIZE, "%02d", minute);
    clockNum.invalidate();

    if (hF)
        Unicode::strncpy(clockTextBuffer, "pm", CLOCKTEXT_SIZE);
    else
        Unicode::strncpy(clockTextBuffer, "am", CLOCKTEXT_SIZE);
    clockText.invalidate();
/*
    Unicode::UnicodeChar dayOfWeekStr[10] = {0};

    switch (dow)
    {
        case 0:
            Unicode::strncpy(dayOfWeekStr, "Monday", 10);
            break;
        case 1:
            Unicode::strncpy(dayOfWeekStr, "Tuesday", 10);
            break;
        case 2:
            Unicode::strncpy(dayOfWeekStr, "Wednesday", 10);
            break;
        case 3:
            Unicode::strncpy(dayOfWeekStr, "Thursday", 10);
            break;
        case 4:
            Unicode::strncpy(dayOfWeekStr, "Friday", 10);
            break;
        case 5:
            Unicode::strncpy(dayOfWeekStr, "Saturday", 10);
            break;
        case 6:
            Unicode::strncpy(dayOfWeekStr, "Sunday", 10);
            break;
    }

    Unicode::snprintf(dayOfWeekBuffer, DAYOFWEEK_SIZE, "%s", dayOfWeekStr);
    dayOfWeek.invalidate();
*/
    
    sunIcon.setVisible(!hideSun);
    sunHorizontImg.setVisible(!hideSun);
    sunIcon.invalidate();
    sunHorizontImg.invalidate();
    
    if (prevSunState != newSunState)
    {      
      sunIcon.clearMoveAnimationEndedAction();
      sunIcon.startMoveAnimation(endX, endY, 48, EasingEquations::linearEaseIn, EasingEquations::linearEaseIn);
      prevSunState = newSunState;
    }
}

void MainView::hideAllContainers(void)
{
  panelSettingsContainer.moveTo(10, -410);
  allScenesContainer.moveTo(-260, 10);
  windowSettingsContainer.moveTo(-260, 10);
  allZonesContainer.moveTo(-260, 10);
  
  backgroundBlur.clearFadeAnimationEndedAction();
  backgroundBlur.setFadeAnimationDelay(8);
  backgroundBlur.startFadeAnimation(0, 4, EasingEquations::linearEaseIn);
    
  windowSettingsButton.setTouchable(true);
  
  panelSettingsButton.setVisible(true);
  panelSettingsButton.setTouchable(true);
}

void MainView::SetStyleHandler(int vall)
{
  if (vall == 2)
  {
    clockNum.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    clockText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    dayOfWeek.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    background.setBitmap(Bitmap(BITMAP_BACKGROUND_IMAGE_ID));
    backgroundBlur.setBitmap(Bitmap(BITMAP_BACKGROUND_IMAGE_BLUR_ID));
    sunIcon.setBitmap(Bitmap(BITMAP_SUN_ICON_ID));
    saintGobainLogo.setBitmap(Bitmap(BITMAP_SAINTGOBAINLOGO_ID));
    sunHorizontImg.setBitmap(Bitmap(BITMAP_COMBINEDGRAPHICNEW_ID));
    panelSettingsButton.setBitmaps(Bitmap(BITMAP_PANELSETTINGSBUTTONICON_ID), Bitmap(BITMAP_PANELSETTINGSBUTTONICON_ID));
  }
  else if (vall == 1)
  {
    clockNum.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    clockText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    dayOfWeek.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    background.setBitmap(Bitmap(BITMAP_BACKGROUNDIMAGEDARKSTYLE_ID));
    backgroundBlur.setBitmap(Bitmap(BITMAP_BACKGROUNDIMAGEDARKSTYLE_ID));
    sunIcon.setBitmap(Bitmap(BITMAP_SUN_ICON_ID));
    saintGobainLogo.setBitmap(Bitmap(BITMAP_SAINTGOBAINLOGO_ID));
    sunHorizontImg.setBitmap(Bitmap(BITMAP_COMBINEDGRAPHICNEW_ID));
    panelSettingsButton.setBitmaps(Bitmap(BITMAP_PANELSETTINGSBUTTONICON_ID), Bitmap(BITMAP_PANELSETTINGSBUTTONICON_ID));
  }
  else if (vall == 0)
  {
    clockNum.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    clockText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    dayOfWeek.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    background.setBitmap(Bitmap(BITMAP_BACKGROUNDIMAGEWHITESTYLE_ID));
    backgroundBlur.setBitmap(Bitmap(BITMAP_BACKGROUNDIMAGEWHITESTYLE_ID));
    sunIcon.setBitmap(Bitmap(BITMAP_SUN_ICON_BLACK_ID));
    saintGobainLogo.setBitmap(Bitmap(BITMAP_SAINTGOBAINLOGOBLACK_ID));
    sunHorizontImg.setBitmap(Bitmap(BITMAP_COMBINED_GRAPHIC_BLACK_ID));
    panelSettingsButton.setBitmaps(Bitmap(BITMAP_PANELSETTINGSBUTTONICONBLACK_ID), Bitmap(BITMAP_PANELSETTINGSBUTTONICON_ID));
  }
  clockNum.invalidate();
  clockText.invalidate();
  dayOfWeek.invalidate();
  background.invalidate();
  backgroundBlur.invalidate();
  sunIcon.invalidate();
  saintGobainLogo.invalidate();
  sunHorizontImg.invalidate();
  panelSettingsButton.invalidate();
  
  panelSettingsContainer.setStyle(vall);
  allScenesContainer.setStyle(vall);
  windowSettingsContainer.setStyle(vall);
  allZonesContainer.setStyle(vall);
}