#ifndef SINGLEZONECONTAINER_HPP
#define SINGLEZONECONTAINER_HPP

#include <gui_generated/containers/SingleZoneContainerBase.hpp>

class SingleZoneContainer : public SingleZoneContainerBase
{
public:
    SingleZoneContainer();
    virtual ~SingleZoneContainer() {}

    virtual void initialize();

    void ResetInfo();
    void SetZoneNameText(Unicode::UnicodeChar *pZoneName);
    void setStyle(int style);
protected:
    virtual void ZoneModeButton0Clicked();
    virtual void ZoneModeButton1Clicked();
    virtual void ZoneModeButton2Clicked();
    virtual void ZoneModeButton3Clicked();
    virtual void ZoneModeButton4Clicked();
    virtual void ZoneModeButton5Clicked();
    virtual void ZoneModeButton6Clicked();
    virtual void ZoneModeButton7Clicked();

private:
    static constexpr uint32_t ZONE_MODE_BUTTONS_COUNT = 8;
    static constexpr uint32_t ZONE_MODE_BUTTON_DEFAULT = 3;
    static constexpr uint32_t ZONE_NAME_TEXT_LENGTH_MAX = 20;
    uint8_t m_lastPressedButtonId;

    touchgfx::ToggleButton *m_pZoneModeButtons[ZONE_MODE_BUTTONS_COUNT];
    Unicode::UnicodeChar m_zoneNameBuffer[ZONE_NAME_TEXT_LENGTH_MAX];

    void ZoneModeButtonClicked(uint32_t buttonId);
};

#endif // SINGLEZONECONTAINER_HPP
