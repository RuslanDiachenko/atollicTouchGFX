#include <gui/main_screen/MainView.hpp>
#include <gui/main_screen/MainPresenter.hpp>

MainPresenter::MainPresenter(MainView& v)
    : view(v)
{
}

void MainPresenter::activate()
{

}

void MainPresenter::deactivate()
{

}

void MainPresenter::notifySunStateChanged(int hour, int minute, int hF, int dow, int date, int month)
{
  view.setSunState(hour, minute, hF, dow, date, month);
}

void MainPresenter::hideAllWidgets(void)
{
  view.hideAllContainers();
}
