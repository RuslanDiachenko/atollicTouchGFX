#ifndef WINDOWSETTINGSCONTAINER_HPP
#define WINDOWSETTINGSCONTAINER_HPP

#include <gui_generated/containers/WindowSettingsContainerBase.hpp>

class WindowSettingsContainer : public WindowSettingsContainerBase
{
public:
    WindowSettingsContainer();
    virtual ~WindowSettingsContainer() {}

    virtual void initialize();

    void setStyle(int style);
    void SetCloseContainerCallback(GenericCallback<void>& callback)
    {
        m_pCloseContainerCallback = &callback;
    }

    void SetOpenManualTintContainerCallback(GenericCallback<void>& callback)
    {
        m_pOpenManualTintContainerCallback = &callback;
    }

    void SetOpenAllScenesContainerCallback(GenericCallback<void>& callback)
    {
        m_pOpenAllScenesContainerCallback = &callback;
    }

protected:
    /*
     * Custom Action Handlers
     */
    virtual void CloseButtonClicked();
    virtual void AutoTintButtonClicked();
    virtual void ManualTintButtonClicked();
    virtual void PresetScenesButtonClicked();

private:
    static constexpr uint32_t SETTINGS_TEXT_SELECTED_ALPHA = 255;
    static constexpr uint32_t SETTINGS_TEXT_NOT_SELECTED_ALPHA = 140;

    GenericCallback<void>* m_pCloseContainerCallback;
    GenericCallback<void>* m_pOpenManualTintContainerCallback;
    GenericCallback<void>* m_pOpenAllScenesContainerCallback;

    enum class PressedButton : uint8_t
    {
        AutoTint,
        ManualTint,
        PresetScenes
    };

    PressedButton m_currentPressedButton;
};

#endif // WINDOWSETTINGSCONTAINER_HPP
