#include <test_app.hpp>

TestApp::TestApp()
{
    m_blinky_timer.set_interrupts(stm32::timer::BasicTimer::DierBits::UIE, 0);
    m_blinky_timer.enable(true);
}

void TestApp::run()
{

    /* Loop forever */
	while(1)
    {

        // delay_ms(100);
        m_blinky_led.toggle();

    }    
}