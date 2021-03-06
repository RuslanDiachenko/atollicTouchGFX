/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef WINDOWSETTINGSCONTAINER_BASE_HPP
#define WINDOWSETTINGSCONTAINER_BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>

class WindowSettingsContainerBase : public touchgfx::Container
{
public:
    WindowSettingsContainerBase();
    virtual ~WindowSettingsContainerBase() {}

    virtual void initialize();

    /*
     * Custom Action Handlers
     */
    virtual void CloseButtonClicked()
    {
        // Override and implement this function in WindowSettingsContainer
    }

    virtual void AutoTintButtonClicked()
    {
        // Override and implement this function in WindowSettingsContainer
    }

    virtual void ManualTintButtonClicked()
    {
        // Override and implement this function in WindowSettingsContainer
    }

    virtual void PresetScenesButtonClicked()
    {
        // Override and implement this function in WindowSettingsContainer
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Image imageBase;
    touchgfx::Button closeButton;
    touchgfx::TextArea containerNameText;
    touchgfx::ToggleButton autoTintButton;
    touchgfx::ToggleButton manualTintButton;
    touchgfx::ToggleButton presetScenesButton;
    touchgfx::TextArea autoTintText;
    touchgfx::TextArea manualTintText;
    touchgfx::TextArea presetScenesText;

private:

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Callback Declarations
     */
    touchgfx::Callback<WindowSettingsContainerBase, const touchgfx::AbstractButton&> buttonCallback;

};

#endif // WINDOWSETTINGSCONTAINER_BASE_HPP
