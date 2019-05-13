/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef PANELSETTINGSCONTAINER_BASE_HPP
#define PANELSETTINGSCONTAINER_BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/Slider.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>

class PanelSettingsContainerBase : public touchgfx::Container
{
public:
    PanelSettingsContainerBase();
    virtual ~PanelSettingsContainerBase() {}

    virtual void initialize();

    /*
     * Custom Action Handlers
     */
    virtual void CloseButtonClicked()
    {
        // Override and implement this function in PanelSettingsContainer
    }

    virtual void sleepAfterSliderCallback(int value)
    {
        // Override and implement this function in PanelSettingsContainer
    }

    virtual void panelBrightnessSliderCallback(int value)
    {
        // Override and implement this function in PanelSettingsContainer
    }

    virtual void urbanStyleButtonClicked()
    {
        // Override and implement this function in PanelSettingsContainer
    }

    virtual void darkStyleButtonClicked()
    {
        // Override and implement this function in PanelSettingsContainer
    }

    virtual void lightStyleButtonClicked()
    {
        // Override and implement this function in PanelSettingsContainer
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Button closeButton;
    touchgfx::TextArea containerNameText;
    touchgfx::TextArea panelBrightnessText;
    touchgfx::Slider sleepAfterSlider;
    touchgfx::TextArea staticTextArea1;
    touchgfx::TextArea staticTextArea2;
    touchgfx::TextArea staticTextArea3;
    touchgfx::TextArea staticTextArea4;
    touchgfx::Slider panelBrightnessSlider;
    touchgfx::TextArea staticTextArea7;
    touchgfx::TextArea staticTextArea8;
    touchgfx::TextArea sleepAfterText;
    touchgfx::TextArea staticTextArea5;
    touchgfx::TextArea staticTextArea6;
    touchgfx::TextArea visualStyleText;
    touchgfx::ToggleButton lightStyleButton;
    touchgfx::ToggleButton darkStyleButton;
    touchgfx::ToggleButton urbanStyleButton;
    touchgfx::TextArea staticTextArea9;
    touchgfx::TextArea staticTextArea10;
    touchgfx::TextArea staticTextArea11;

private:

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void sliderValueConfirmedCallbackHandler(const touchgfx::Slider& src, int value);

    /*
     * Callback Declarations
     */
    touchgfx::Callback<PanelSettingsContainerBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<PanelSettingsContainerBase, const touchgfx::Slider&, int> sliderValueConfirmedCallback;

};

#endif // PANELSETTINGSCONTAINER_BASE_HPP