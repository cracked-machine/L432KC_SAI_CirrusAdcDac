
#ifndef __system_device_hpp__
#define __system_device_hpp__


#if defined (USE_HAL_DRIVER)
    #undef USE_HAL_DRIVER
#endif
#include <stm32l4xx.h>


#include <gpio.hpp>

namespace stm32 {

class SystemDevice
{
public:
    /// @brief Initialize the system device clocks
    SystemDevice();

    void run();
    
    /// @brief delay the system clock
    /// @param ms The time to delay in milliseconds
    void delay_ms(uint32_t ms);

    gpio::Driver m_blinky_led{
        GPIOB, 
        gpio::Pin::PIN3, 
        gpio::Mode::OUTPUT, 
        gpio::Otype::PUSHPULL, 
        gpio::Speed::VHIGH,
        gpio::Pupd::PU};
};

} // namespace stm32

#endif // __system_device_hpp__