#include <gui/containers/SingleZoneContainer.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

SingleZoneContainer::SingleZoneContainer()
{
    m_pZoneModeButtons[0] = &zoneModeButton0;
    m_pZoneModeButtons[1] = &zoneModeButton1;
    m_pZoneModeButtons[2] = &zoneModeButton2;
    m_pZoneModeButtons[3] = &zoneModeButton3;
    m_pZoneModeButtons[4] = &zoneModeButton4;
    m_pZoneModeButtons[5] = &zoneModeButton5;
    m_pZoneModeButtons[6] = &zoneModeButton6;
    m_pZoneModeButtons[7] = &zoneModeButton7;

    m_pZoneModeButtons[ZONE_MODE_BUTTON_DEFAULT]->forceState(true);
    ZoneModeButtonClicked(ZONE_MODE_BUTTON_DEFAULT);

    Unicode::snprintf(m_zoneNameBuffer, 20, "%s", Unicode::EMPTY);
    zoneNameText.setWildcard(m_zoneNameBuffer);
    
    m_lastPressedButtonId = ZONE_MODE_BUTTON_DEFAULT;
}

void SingleZoneContainer::initialize()
{
    SingleZoneContainerBase::initialize();
}

void SingleZoneContainer::ResetInfo()
{
    m_pZoneModeButtons[ZONE_MODE_BUTTON_DEFAULT]->forceState(true);
    ZoneModeButtonClicked(ZONE_MODE_BUTTON_DEFAULT);
}

void SingleZoneContainer::SetZoneNameText(Unicode::UnicodeChar *pZoneName)
{
    Unicode::snprintf(m_zoneNameBuffer, 20, "%s", pZoneName);
    zoneNameText.invalidate();
}

void SingleZoneContainer::ZoneModeButtonClicked(uint32_t buttonId)
{
    if (m_pZoneModeButtons[buttonId]->getState())
    {
        m_lastPressedButtonId = buttonId;
        m_pZoneModeButtons[buttonId]->setTouchable(false);

        for (uint32_t i = 0; i < ZONE_MODE_BUTTONS_COUNT; ++i)
        {
            if ((i != buttonId) && m_pZoneModeButtons[i]->getState())
            {
                m_pZoneModeButtons[i]->setTouchable(true);
                m_pZoneModeButtons[i]->forceState(false);
                m_pZoneModeButtons[i]->invalidate();
            }
        }
    }
}

void SingleZoneContainer::ZoneModeButton0Clicked()
{
    ZoneModeButtonClicked(0);
}

void SingleZoneContainer::ZoneModeButton1Clicked()
{
    ZoneModeButtonClicked(1);
}

void SingleZoneContainer::ZoneModeButton2Clicked()
{
    ZoneModeButtonClicked(2);
}

void SingleZoneContainer::ZoneModeButton3Clicked()
{
    ZoneModeButtonClicked(3);
}

void SingleZoneContainer::ZoneModeButton4Clicked()
{
    ZoneModeButtonClicked(4);
}

void SingleZoneContainer::ZoneModeButton5Clicked()
{
    ZoneModeButtonClicked(5);
}

void SingleZoneContainer::ZoneModeButton6Clicked()
{
    ZoneModeButtonClicked(6);
}

void SingleZoneContainer::ZoneModeButton7Clicked()
{
    ZoneModeButtonClicked(7);
}

void SingleZoneContainer::setStyle(int style)
{
  if (style == 2)
  {
    zoneNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    m_pZoneModeButtons[0]->setBitmaps(Bitmap(BITMAP_ZONEGREY3NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY3PRESSED_ID));
    m_pZoneModeButtons[1]->setBitmaps(Bitmap(BITMAP_ZONEGREY2NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY2PRESSED_ID));
    m_pZoneModeButtons[2]->setBitmaps(Bitmap(BITMAP_ZONEGREY1NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY1PRESSED_ID));
    m_pZoneModeButtons[3]->setBitmaps(Bitmap(BITMAP_ZONEWHITENOTPRESSED_ID), Bitmap(BITMAP_ZONEWHITEPRESSED_ID));
    m_pZoneModeButtons[4]->setBitmaps(Bitmap(BITMAP_ZONEGRAD4NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD4PRESSED_ID));
    m_pZoneModeButtons[5]->setBitmaps(Bitmap(BITMAP_ZONEGRAD3NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD3PRESSED_ID));
    m_pZoneModeButtons[6]->setBitmaps(Bitmap(BITMAP_ZONEGRAD2NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD2PRESSED_ID));
    m_pZoneModeButtons[7]->setBitmaps(Bitmap(BITMAP_ZONEGRAD1NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD1PRESSED_ID));
  }
  else if (style == 1)
  {
    zoneNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    m_pZoneModeButtons[0]->setBitmaps(Bitmap(BITMAP_ZONEGREY3NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY3PRESSED_ID));
    m_pZoneModeButtons[1]->setBitmaps(Bitmap(BITMAP_ZONEGREY2NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY2PRESSED_ID));
    m_pZoneModeButtons[2]->setBitmaps(Bitmap(BITMAP_ZONEGREY1NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY1PRESSED_ID));
    m_pZoneModeButtons[3]->setBitmaps(Bitmap(BITMAP_ZONEWHITENOTPRESSED_ID), Bitmap(BITMAP_ZONEWHITEPRESSED_ID));
    m_pZoneModeButtons[4]->setBitmaps(Bitmap(BITMAP_ZONEGRAD4NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD4PRESSED_ID));
    m_pZoneModeButtons[5]->setBitmaps(Bitmap(BITMAP_ZONEGRAD3NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD3PRESSED_ID));
    m_pZoneModeButtons[6]->setBitmaps(Bitmap(BITMAP_ZONEGRAD2NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD2PRESSED_ID));
    m_pZoneModeButtons[7]->setBitmaps(Bitmap(BITMAP_ZONEGRAD1NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD1PRESSED_ID));
  }
  else if (style == 0)
  {
    zoneNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    m_pZoneModeButtons[0]->setBitmaps(Bitmap(BITMAP_ZONEGREY3NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY3PRESSEDBLACK_ID));
    m_pZoneModeButtons[1]->setBitmaps(Bitmap(BITMAP_ZONEGREY2NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY2PRESSEDBLACK_ID));
    m_pZoneModeButtons[2]->setBitmaps(Bitmap(BITMAP_ZONEGREY1NOTPRESSED_ID), Bitmap(BITMAP_ZONEGREY1PRESSEDBLACK_ID));
    m_pZoneModeButtons[3]->setBitmaps(Bitmap(BITMAP_ZONEWHITENOTPRESSED_ID), Bitmap(BITMAP_ZONEWHITEPRESSEDBLACK_ID));
    m_pZoneModeButtons[4]->setBitmaps(Bitmap(BITMAP_ZONEGRAD4NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD4PRESSEDBLACK_ID));
    m_pZoneModeButtons[5]->setBitmaps(Bitmap(BITMAP_ZONEGRAD3NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD3PRESSEDBLACK_ID));
    m_pZoneModeButtons[6]->setBitmaps(Bitmap(BITMAP_ZONEGRAD2NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD2PRESSEDBLACK_ID));
    m_pZoneModeButtons[7]->setBitmaps(Bitmap(BITMAP_ZONEGRAD1NOTPRESSED_ID), Bitmap(BITMAP_ZONEGRAD1PRESSEDBLACK_ID));
  }
  
  zoneNameText.invalidate();
  
  for (uint8_t i = 0; i < 8; i++)
  {
    m_pZoneModeButtons[i]->forceState((i == m_lastPressedButtonId) ? true : false);
    m_pZoneModeButtons[i]->invalidate();
  }
}