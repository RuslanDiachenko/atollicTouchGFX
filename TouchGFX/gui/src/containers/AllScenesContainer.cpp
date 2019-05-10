#include <gui/containers/AllScenesContainer.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

AllScenesContainer::AllScenesContainer()
{
    m_pSceneButtons[0] = &sceneButton0;
    m_pSceneButtons[1] = &sceneButton1;
    m_pSceneButtons[2] = &sceneButton2;
    m_pSceneButtons[3] = &sceneButton3;
    m_pSceneButtons[4] = &sceneButton4;

    m_pSceneButtonsText[0] = &sceneNameText0;
    m_pSceneButtonsText[1] = &sceneNameText1;
    m_pSceneButtonsText[2] = &sceneNameText2;
    m_pSceneButtonsText[3] = &sceneNameText3;
    m_pSceneButtonsText[4] = &sceneNameText4;

    m_pSceneButtons[DEFAULT_SELECTED_BUTTON]->forceState(true);
    SceneButtonClicked(DEFAULT_SELECTED_BUTTON);
    
    m_lastPressedButtonId = DEFAULT_SELECTED_BUTTON;
}

void AllScenesContainer::initialize()
{
    AllScenesContainerBase::initialize();
}

void AllScenesContainer::CloseButtonClicked()
{
    m_pCloseContainerCallback->execute();
}

void AllScenesContainer::BackButtonClicked()
{
    m_pBackContainerCallback->execute();
}

void AllScenesContainer::SceneButtonClicked(uint8_t buttonId)
{
    if (m_pSceneButtons[buttonId]->getState())
    {
        m_lastPressedButtonId = buttonId;
        m_pSceneButtons[buttonId]->setTouchable(false);

        m_pSceneButtonsText[buttonId]->setAlpha(SCENE_TEXT_SELECTED_ALPHA);
        m_pSceneButtonsText[buttonId]->invalidate();

        for (uint32_t i = 0; i < SCENE_BUTTONS_COUNT; ++i)
        {
            if ((i != buttonId) && m_pSceneButtons[i]->getState())
            {
                m_pSceneButtons[i]->setTouchable(true);
                m_pSceneButtons[i]->forceState(false);
                m_pSceneButtons[i]->invalidate();

                m_pSceneButtonsText[i]->setAlpha(SCENE_TEXT_NOT_SELECTED_ALPHA);
                m_pSceneButtonsText[i]->invalidate();
            }
        }
    }
}

void AllScenesContainer::SceneButton0Clicked()
{
    SceneButtonClicked(0);
}

void AllScenesContainer::SceneButton1Clicked()
{
    SceneButtonClicked(1);
}

void AllScenesContainer::SceneButton2Clicked()
{
    SceneButtonClicked(2);
}

void AllScenesContainer::SceneButton3Clicked()
{
    SceneButtonClicked(3);
}

void AllScenesContainer::SceneButton4Clicked()
{
    SceneButtonClicked(4);
}

void AllScenesContainer::setStyle(int style)
{
  if (style == 2)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    for (uint8_t i = 0; i < SCENE_BUTTONS_COUNT; i++)
      m_pSceneButtonsText[i]->setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    for (uint8_t i = 0; i < SCENE_BUTTONS_COUNT; i++)
      m_pSceneButtons[i]->setBitmaps(Bitmap(BITMAP_TOGGLENOTPRESSED_ID), Bitmap(BITMAP_TOGGLEPRESSED_ID));
  }
  else if (style == 1)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    for (uint8_t i = 0; i < SCENE_BUTTONS_COUNT; i++)
      m_pSceneButtonsText[i]->setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    
    for (uint8_t i = 0; i < SCENE_BUTTONS_COUNT; i++)
      m_pSceneButtons[i]->setBitmaps(Bitmap(BITMAP_TOGGLENOTPRESSED_ID), Bitmap(BITMAP_TOGGLEPRESSED_ID));
  }
  else if (style == 0)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    for (uint8_t i = 0; i < SCENE_BUTTONS_COUNT; i++)
      m_pSceneButtonsText[i]->setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    
    for (uint8_t i = 0; i < SCENE_BUTTONS_COUNT; i++)
      m_pSceneButtons[i]->setBitmaps(Bitmap(BITMAP_TOGGLENOTPRESSED_ID), Bitmap(BITMAP_TOGGLEPRESSEDBLACK_ID));
  }
  
  containerNameText.invalidate();
  
  for (uint32_t i = 0; i < SCENE_BUTTONS_COUNT; ++i)
  {
    m_pSceneButtons[i]->forceState((i == m_lastPressedButtonId) ? true : false);
    m_pSceneButtons[i]->invalidate();
    m_pSceneButtonsText[i]->invalidate();
  }  
  
}