/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/AllZonesContainerBase.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

AllZonesContainerBase::AllZonesContainerBase() :
    buttonCallback(this, &AllZonesContainerBase::buttonCallbackHandler)
{
    setWidth(255);
    setHeight(411);

    background.setXY(0, 0);
    background.setBitmap(Bitmap(BITMAP_PANELBASE_ID));

    closeButton.setXY(210, 3);
    closeButton.setBitmaps(Bitmap(BITMAP_CLOSEBUTTON_ID), Bitmap(BITMAP_CLOSEBUTTON_ID));
    closeButton.setAction(buttonCallback);

    scrollableAllZonesContainer.setPosition(6, 71, 244, 334);
    scrollableAllZonesContainer.enableHorizontalScroll(false);
    scrollableAllZonesContainer.setScrollbarsColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));


    scrollableAllZonesContainer.setScrollbarsVisible(false);

    backButton.setXY(6, 3);
    backButton.setBitmaps(Bitmap(BITMAP_BACKBUTTON_ID), Bitmap(BITMAP_BACKBUTTON_ID));
    backButton.setAction(buttonCallback);

    containerNameText.setXY(97, 11);
    containerNameText.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    containerNameText.setLinespacing(0);
    containerNameText.setTypedText(TypedText(T_SINGLEUSEID11));

    add(background);
    add(closeButton);
    add(scrollableAllZonesContainer);
    add(backButton);
    add(containerNameText);
}

void AllZonesContainerBase::initialize()
{
	
}

void AllZonesContainerBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &closeButton)
    {
        //CloseButtonClicked
        //When closeButton clicked call virtual function
        //Call CloseButtonClicked
        CloseButtonClicked();
    }
    else if (&src == &backButton)
    {
        //BackButtonClicked
        //When backButton clicked call virtual function
        //Call BackButtonClicked
        BackButtonClicked();
    }
}
