#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>

class MainView : public MainViewBase
{
public:

    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void setSunState(int hour, int minute, int hF, int dow);
    void hideAllContainers(void);
protected:
    virtual void WindowSettingsButtonClicked();
    virtual void PanelSettingsButtonClicked();

    void OpenAllZonesContainerHandler(void);
    void CloseAllZonesContainerHandler(void);
    void BackAllZonesContainerHandler(void);
    void OpenAllScenesContainerHandler(void);
    void CloseAllScenesContainerHandler(void);
    void BackAllScenesContainerHandler(void);
    void CloseWindowSettingsContainerHandler(void);
    void ClosePanelSettingsContainerHandler(void);  
    void SetStyleHandler(int vall);
    
    touchgfx::Callback<MainView, void> openAllZonesContainer;
    touchgfx::Callback<MainView, void> closeAllZonesContainer;
    touchgfx::Callback<MainView, void> backAllZonesContainer;
    touchgfx::Callback<MainView, void> openAllScenesContainer;
    touchgfx::Callback<MainView, void> closeAllScenesContainer;
    touchgfx::Callback<MainView, void> backAllScenesContainer;
    touchgfx::Callback<MainView, void> closeWindowSettingsContainer;
    touchgfx::Callback<MainView, void> closePanelSettingsContainer;
    touchgfx::Callback<MainView, int> setStyle;

    uint8_t m_lastBackgroundBlurAlfa;
    bool m_lastWindowSettingsButtonTouchable;
};

#endif // MAIN_VIEW_HPP
