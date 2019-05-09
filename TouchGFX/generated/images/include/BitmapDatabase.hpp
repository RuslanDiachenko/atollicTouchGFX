// Generated by imageconverter. Please, do not edit!

#ifndef BITMAPDATABASE_HPP
#define BITMAPDATABASE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>

const uint16_t BITMAP_BACKGROUND_IMAGE_ID = 0; // Size: 272x480 pixels
const uint16_t BITMAP_BACKGROUND_IMAGE_BLUR_ID = 1; // Size: 272x480 pixels
const uint16_t BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_ID = 2; // Size: 60x60 pixels
const uint16_t BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_PRESSED_ID = 3; // Size: 60x60 pixels

namespace BitmapDatabase
{
#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

class BitmapData;
const touchgfx::Bitmap::BitmapData* getInstance();
uint16_t getInstanceSize();
}

#endif
