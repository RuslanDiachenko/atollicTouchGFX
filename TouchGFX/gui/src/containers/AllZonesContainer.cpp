#include <gui/containers/AllZonesContainer.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

AllZonesContainer::AllZonesContainer()
{
    for (uint32_t i = 0; i < ZONE_CONTAINER_COUNT_MAX; ++i)
    {
        m_zonesInfos[i].isUsed = false;
    }

    m_lastZoneContainerEndY = 0;

    //////////////////////////////////////
    Unicode::UnicodeChar zoneName[20];

    Unicode::strncpy(zoneName, "Zone 1", 20);
    AddNewZone(ZoneType::Type8, zoneName);

    Unicode::strncpy(zoneName, "Zone 2", 20);
    AddNewZone(ZoneType::Type4, zoneName);

    Unicode::strncpy(zoneName, "Zone 3", 20);
    AddNewZone(ZoneType::Type8, zoneName);

    Unicode::strncpy(zoneName, "Zone 4", 20);
    AddNewZone(ZoneType::Type8, zoneName);
    /////////////////////////////////////
}

void AllZonesContainer::initialize()
{
    AllZonesContainerBase::initialize();
}

void AllZonesContainer::CloseButtonClicked()
{
    scrollableAllZonesContainer.reset();
    m_pCloseContainerCallback->execute();
}

void AllZonesContainer::BackButtonClicked()
{
    scrollableAllZonesContainer.reset();
    m_pBackContainerCallback->execute();
}

void AllZonesContainer::ResetButtonClicked()
{
    DeleteAllZones();

    Unicode::UnicodeChar zoneName[20];

    Unicode::strncpy(zoneName, "Zone 5", 20);
    AddNewZone(ZoneType::Type8, zoneName);

    Unicode::strncpy(zoneName, "Zone 6", 20);
    AddNewZone(ZoneType::Type4, zoneName);

    Unicode::strncpy(zoneName, "Zone 7", 20);
    AddNewZone(ZoneType::Type4, zoneName);

    Unicode::strncpy(zoneName, "Zone 8", 20);
    AddNewZone(ZoneType::Type8, zoneName);
}

bool AllZonesContainer::AddNewZone(ZoneType type, Unicode::UnicodeChar *pZoneName)
{
    uint16_t freeContainerInfoIndex = GetFirstFreeContainerInfoIndex();
    if (freeContainerInfoIndex == ZONE_CONTAINER_COUNT_MAX)
    {
        return false;
    }

    uint16_t zoneHeight = (type == ZoneType::Type4) ? ZONE_CONTAINER_4_HEIGHT : ZONE_CONTAINER_8_HEIGHT;

    m_zonesInfos[freeContainerInfoIndex].isUsed = true;
    m_zonesInfos[freeContainerInfoIndex].type = (uint8_t)type;

    m_zonesInfos[freeContainerInfoIndex].zoneContainer.SetZoneNameText(pZoneName);
    m_zonesInfos[freeContainerInfoIndex].zoneContainer.setHeight(zoneHeight);
    m_zonesInfos[freeContainerInfoIndex].zoneContainer.setY(m_lastZoneContainerEndY);
    scrollableAllZonesContainer.add(m_zonesInfos[freeContainerInfoIndex].zoneContainer);

    scrollableAllZonesContainer.childGeometryChanged();
    scrollableAllZonesContainer.invalidate();

    m_lastZoneContainerEndY += zoneHeight;

    return true;
}

void AllZonesContainer::DeleteAllZones()
{
    scrollableAllZonesContainer.reset();
    scrollableAllZonesContainer.removeAll();
    scrollableAllZonesContainer.childGeometryChanged();
    scrollableAllZonesContainer.invalidate();

    m_lastZoneContainerEndY = 0;

    for (uint32_t i = 0; i < ZONE_CONTAINER_COUNT_MAX; ++i)
    {
        if (m_zonesInfos[i].isUsed)
        {
            m_zonesInfos[i].isUsed = false;
            m_zonesInfos[i].zoneContainer.ResetInfo();
        }
        else
        {
            break;
        }
    }
}

uint16_t AllZonesContainer::GetFirstFreeContainerInfoIndex()
{
    for (uint32_t i = 0; i < ZONE_CONTAINER_COUNT_MAX; ++i)
    {
        if (!m_zonesInfos[i].isUsed)
        {
            return i;
        }
    }

    return ZONE_CONTAINER_COUNT_MAX;
}

void AllZonesContainer::setStyle(int style)
{
  if (style == 2)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
  }
  else if (style == 1)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
  }
  else if (style == 0)
  {
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
  }
  containerNameText.invalidate();
  
  for (uint8_t i = 0; i < GetFirstFreeContainerInfoIndex(); i++)
  {
    m_zonesInfos[i].zoneContainer.setStyle(style);
  }
}