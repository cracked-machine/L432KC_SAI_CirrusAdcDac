
#ifndef __system_device_hpp__
#define __system_device_hpp__


#if defined (USE_HAL_DRIVER)
    #undef USE_HAL_DRIVER
#endif
#include <stm32l4xx.h>
#include <reg_macros.h>

class SystemDevice
{
public:
    /// @brief Initialize the system device clocks
    SystemDevice();

    void run();
    
    /// @brief delay the system clock
    /// @param ms The time to delay in milliseconds
    void delay_ms(uint32_t ms);
};

#endif // __system_device_hpp__