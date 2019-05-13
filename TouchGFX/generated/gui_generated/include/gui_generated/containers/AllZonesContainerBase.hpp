/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef ALLZONESCONTAINER_BASE_HPP
#define ALLZONESCONTAINER_BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>

#include <touchgfx/widgets/TextArea.hpp>
class AllZonesContainerBase : public touchgfx::Container
{
public:
    AllZonesContainerBase();
    virtual ~AllZonesContainerBase() {}

    virtual void initialize();

    /*
     * Custom Action Handlers
     */
    virtual void CloseButtonClicked()
    {
        // Override and implement this function in AllZonesContainer
    }

    virtual void BackButtonClicked()
    {
        // Override and implement this function in AllZonesContainer
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Image background;
    touchgfx::Button closeButton;
    touchgfx::ScrollableContainer scrollableAllZonesContainer;


    touchgfx::Button backButton;
    touchgfx::TextArea containerNameText;

private:

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Callback Declarations
     */
    touchgfx::Callback<AllZonesContainerBase, const touchgfx::AbstractButton&> buttonCallback;

};

#endif // ALLZONESCONTAINER_BASE_HPP