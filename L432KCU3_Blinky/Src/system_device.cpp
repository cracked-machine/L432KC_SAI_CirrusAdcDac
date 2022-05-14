#include <system_device.hpp>

/// @brief The system clock tick
volatile uint32_t g_systick {0};

namespace stm32 {

SystemDevice::SystemDevice()
{

    // Configure the SysTick to have interrupt in 1ms time basis
    uint32_t tick_freq_1khz = 1U;
    SysTick_Config(SystemCoreClock / (1000U / tick_freq_1khz));
    fpu_type = SCB_GetFPUType();
   
}

void SystemDevice::run()
{

    /* Loop forever */
	while(1)
    {

        delay_ms(100);
        m_blinky_led.set_pin();
        delay_ms(100);
        m_blinky_led.reset_pin();
    }    
}

void SystemDevice::delay_ms(uint32_t ms) {
    uint32_t start = g_systick;
    while (g_systick - start < ms);
}    


} // namespace stm32
