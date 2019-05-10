#include <gui/containers/WindowSettingsContainer.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

WindowSettingsContainer::WindowSettingsContainer() :
    m_currentPressedButton(PressedButton::AutoTint)
{
    autoTintButton.forceState(true);
    AutoTintButtonClicked();
}

void WindowSettingsContainer::initialize()
{
    WindowSettingsContainerBase::initialize();
}

void WindowSettingsContainer::CloseButtonClicked()
{
    m_pCloseContainerCallback->execute();
}

void WindowSettingsContainer::AutoTintButtonClicked()
{
    if (autoTintButton.getState())
    {
        m_currentPressedButton = PressedButton::AutoTint;

        autoTintButton.setTouchable(false);
        autoTintText.setAlpha(SETTINGS_TEXT_SELECTED_ALPHA);
        autoTintText.invalidate();

        manualTintButton.forceState(false);
        manualTintButton.invalidate();
        manualTintText.setAlpha(SETTINGS_TEXT_NOT_SELECTED_ALPHA);
        manualTintText.invalidate();

        presetScenesButton.forceState(false);
        presetScenesButton.invalidate();
        presetScenesText.setAlpha(SETTINGS_TEXT_NOT_SELECTED_ALPHA);
        presetScenesText.invalidate();
    }
}

void WindowSettingsContainer::ManualTintButtonClicked()
{
    if (manualTintButton.getState())
    {
        m_currentPressedButton = PressedButton::ManualTint;

        manualTintText.setAlpha(SETTINGS_TEXT_SELECTED_ALPHA);
        manualTintText.invalidate();

        autoTintButton.setTouchable(true);
        autoTintButton.forceState(false);
        autoTintButton.invalidate();
        autoTintText.setAlpha(SETTINGS_TEXT_NOT_SELECTED_ALPHA);
        autoTintText.invalidate();

        presetScenesButton.forceState(false);
        presetScenesButton.invalidate();
        presetScenesText.setAlpha(SETTINGS_TEXT_NOT_SELECTED_ALPHA);
        presetScenesText.invalidate();

        m_pOpenManualTintContainerCallback->execute();
    }
    else if(m_currentPressedButton == PressedButton::ManualTint)
    {
        manualTintButton.forceState(true);
        manualTintButton.invalidate();

        m_pOpenManualTintContainerCallback->execute();
    }
}

void WindowSettingsContainer::PresetScenesButtonClicked()
{
    if (presetScenesButton.getState())
    {
        m_currentPressedButton = PressedButton::PresetScenes;

        presetScenesText.setAlpha(SETTINGS_TEXT_SELECTED_ALPHA);
        presetScenesText.invalidate();

        autoTintButton.setTouchable(true);
        autoTintButton.forceState(false);
        autoTintButton.invalidate();
        autoTintText.setAlpha(SETTINGS_TEXT_NOT_SELECTED_ALPHA);
        autoTintText.invalidate();

        manualTintButton.forceState(false);
        manualTintButton.invalidate();
        manualTintText.setAlpha(SETTINGS_TEXT_NOT_SELECTED_ALPHA);
        manualTintText.invalidate();

        m_pOpenAllScenesContainerCallback->execute();
    }
    else if(m_currentPressedButton == PressedButton::PresetScenes)
    {
        presetScenesButton.forceState(true);
        presetScenesButton.invalidate();

        m_pOpenAllScenesContainerCallback->execute();
    }
}

void WindowSettingsContainer::setStyle(int style)
{
  if (style == 2)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    autoTintText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    manualTintText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    presetScenesText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    autoTintButton.setBitmaps(Bitmap(BITMAP_AUTOTINTBUTTONNOTPRESSEDNEW_ID), Bitmap(BITMAP_AUTOTINTBUTTONPRESSEDNEW_ID));
    manualTintButton.setBitmaps(Bitmap(BITMAP_MANUALTINTBUTTONNOTPRESSEDNEW_ID), Bitmap(BITMAP_MANUALTINTBUTTONPRESSEDNEW_ID));
    presetScenesButton.setBitmaps(Bitmap(BITMAP_PRESENTSCENESBUTTONNOTPRESSEDNEW_ID), Bitmap(BITMAP_PRESENTSCENESBUTTONPRESSEDNEW_ID));
  }
  else if (style == 1)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    autoTintText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    manualTintText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    presetScenesText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    autoTintButton.setBitmaps(Bitmap(BITMAP_AUTOTINTBUTTONNOTPRESSEDNEW_ID), Bitmap(BITMAP_AUTOTINTBUTTONPRESSEDNEW_ID));
    manualTintButton.setBitmaps(Bitmap(BITMAP_MANUALTINTBUTTONNOTPRESSEDNEW_ID), Bitmap(BITMAP_MANUALTINTBUTTONPRESSEDNEW_ID));
    presetScenesButton.setBitmaps(Bitmap(BITMAP_PRESENTSCENESBUTTONNOTPRESSEDNEW_ID), Bitmap(BITMAP_PRESENTSCENESBUTTONPRESSEDNEW_ID));
  }
  else if (style == 0)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    autoTintText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    manualTintText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    presetScenesText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    
    autoTintButton.setBitmaps(Bitmap(BITMAP_AUTOTINTBUTTONNOTPRESSEDBLACK_ID), Bitmap(BITMAP_AUTOTINTBUTTONPRESSEDBLACK_ID));
    manualTintButton.setBitmaps(Bitmap(BITMAP_MANUALTINTBUTTONNOTPRESSEDBLACK_ID), Bitmap(BITMAP_MANUALTINTBUTTONPRESSEDBLACK_ID));
    presetScenesButton.setBitmaps(Bitmap(BITMAP_PRESENTSCENESBUTTONNOTPRESSEDBLACK_ID), Bitmap(BITMAP_PRESENTSCENESBUTTONPRESSEDBLACK_ID));
  }
  
  if(m_currentPressedButton == PressedButton::AutoTint)
  {
    autoTintButton.forceState(true);
    manualTintButton.forceState(false);
    presetScenesButton.forceState(false);
  }
  else if (m_currentPressedButton == PressedButton::ManualTint)
  {
    autoTintButton.forceState(false);
    manualTintButton.forceState(true);
    presetScenesButton.forceState(false);
  }
  else if (m_currentPressedButton == PressedButton::PresetScenes)
  {
    autoTintButton.forceState(false);
    manualTintButton.forceState(false);
    presetScenesButton.forceState(true);
  }
  
  containerNameText.invalidate();
  autoTintText.invalidate();
  manualTintText.invalidate();
  presetScenesText.invalidate();
  
  autoTintButton.invalidate();
  manualTintButton.invalidate();
  presetScenesButton.invalidate();
}