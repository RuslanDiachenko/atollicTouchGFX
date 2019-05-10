#ifndef __CUSTOM_HAL_HPP__
#define __CUSTOM_HAL_HPP__

#include "STM32F4HAL.hpp"

class CustomHAL : public STM32F4HAL
{
public:
    CustomHAL(touchgfx::DMA_Interface& dma, touchgfx::LCD& display, touchgfx::TouchController& tc, uint16_t width, uint16_t height) : STM32F4HAL(dma, display, tc, width, height)
    {
    }

    virtual ~CustomHAL() {};

    virtual bool blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes);

};

#endif // #ifndef __CUSTOM_HAL_HPP__
