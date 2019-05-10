#ifndef ALLZONESCONTAINER_HPP
#define ALLZONESCONTAINER_HPP

#include <gui_generated/containers/AllZonesContainerBase.hpp>
#include <gui/containers/SingleZoneContainer.hpp>

enum class ZoneType : uint8_t
{
    Type4 = 0,
    Type8 = 1
};

class AllZonesContainer : public AllZonesContainerBase
{
public:
    AllZonesContainer();
    virtual ~AllZonesContainer() {}

    virtual void initialize();

    void SetCloseContainerCallback(GenericCallback<void>& callback)
    {
        m_pCloseContainerCallback = &callback;
    }

    void SetBackContainerCallback(GenericCallback<void>& callback)
    {
        m_pBackContainerCallback = &callback;
    }

    bool AddNewZone(ZoneType type, Unicode::UnicodeChar *pZoneName);
    void DeleteAllZones();
    
    void setStyle(int style);
protected:
    virtual void CloseButtonClicked();
    virtual void ResetButtonClicked();
    virtual void BackButtonClicked();

private:
    uint16_t GetFirstFreeContainerInfoIndex();

    static constexpr uint32_t ZONE_CONTAINER_COUNT_MAX = 100;
    static constexpr uint32_t ZONE_CONTAINER_4_HEIGHT = 110;
    static constexpr uint32_t ZONE_CONTAINER_8_HEIGHT = 180;

    struct ZoneInfos
    {
        SingleZoneContainer zoneContainer;
        uint8_t isUsed : 1;
        uint8_t type : 1;
    };

    GenericCallback<void>* m_pCloseContainerCallback;
    GenericCallback<void>* m_pBackContainerCallback;

    ZoneInfos m_zonesInfos[ZONE_CONTAINER_COUNT_MAX];
    uint16_t m_lastZoneContainerEndY;
};

#endif // ALLZONESCONTAINER_HPP
