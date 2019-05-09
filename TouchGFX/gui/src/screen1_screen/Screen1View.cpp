#include <gui/screen1_screen/Screen1View.hpp>

uint8_t radioactiveState = 0;

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::button1Callback()
{
	image.clearMoveAnimationEndedAction();
	if (radioactiveState)
	{
		image.startMoveAnimation(299, 0, 30, EasingEquations::backEaseInOut, EasingEquations::backEaseInOut);
		radioactiveState = 0;
	}
	else
	{
		image.startMoveAnimation(0, 0, 30, EasingEquations::bounceEaseInOut, EasingEquations::bounceEaseInOut);
		radioactiveState = 1;
	}
}
