#ifndef PANELSETTINGSCONTAINER_HPP
#define PANELSETTINGSCONTAINER_HPP

#include <gui_generated/containers/PanelSettingsContainerBase.hpp>

class PanelSettingsContainer : public PanelSettingsContainerBase
{
public:
    PanelSettingsContainer();
    virtual ~PanelSettingsContainer() {}

    virtual void initialize();

    void setStyle(int style);
    
    virtual void urbanStyleButtonClicked(void);
    virtual void darkStyleButtonClicked(void);
    virtual void lightStyleButtonClicked(void);
    
    virtual void panelBrightnessSliderCallback(int value);
    virtual void sleepAfterSliderCallback(int value);
    void startUrbanStyle(void);
    void SetCloseContainerCallback(GenericCallback<void> &callback)
    {
        m_pCloseContainerCallback = &callback;
    }
    
    void SetStyleCallback(GenericCallback<int> &callback)
    {
        m_pSetStyleCallback = &callback;
    }

protected:
    virtual void CloseButtonClicked();

private:
    GenericCallback<int> *m_pSetStyleCallback; 
    GenericCallback<void> *m_pCloseContainerCallback;
    
    enum class PressedButton : uint8_t
    {
        LightStyle,
        DarkStyle,
        UrbanStyle
    };
    
    PressedButton m_currentPressedButton;
};

#endif // PANELSETTINGSCONTAINER_HPP
