#ifndef ALLSCENESCONTAINER_HPP
#define ALLSCENESCONTAINER_HPP

#include <gui_generated/containers/AllScenesContainerBase.hpp>

class AllScenesContainer : public AllScenesContainerBase
{
public:
    AllScenesContainer();
    virtual ~AllScenesContainer() {}

    virtual void initialize();

    void setStyle(int style);
    
    void SetCloseContainerCallback(GenericCallback<void>& callback)
    {
        m_pCloseContainerCallback = &callback;
    }

    void SetBackContainerCallback(GenericCallback<void>& callback)
    {
        m_pBackContainerCallback = &callback;
    }


protected:
    virtual void CloseButtonClicked();
    virtual void BackButtonClicked();

    virtual void SceneButton0Clicked();
    virtual void SceneButton1Clicked();
    virtual void SceneButton2Clicked();
    virtual void SceneButton3Clicked();
    virtual void SceneButton4Clicked();

private:
    void SceneButtonClicked(uint8_t buttonId);

    static constexpr uint32_t SCENE_BUTTONS_COUNT = 5;
    static constexpr uint32_t DEFAULT_SELECTED_BUTTON = 0;
    static constexpr uint32_t SCENE_TEXT_SELECTED_ALPHA = 255;
    static constexpr uint32_t SCENE_TEXT_NOT_SELECTED_ALPHA = 140;

    GenericCallback<void>* m_pCloseContainerCallback;
    GenericCallback<void>* m_pBackContainerCallback;

    touchgfx::ToggleButton *m_pSceneButtons[SCENE_BUTTONS_COUNT];
    touchgfx::TextArea *m_pSceneButtonsText[SCENE_BUTTONS_COUNT];
    
    uint8_t m_lastPressedButtonId;
};

#endif // ALLSCENESCONTAINER_HPP
