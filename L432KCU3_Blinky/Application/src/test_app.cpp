#include <test_app.hpp>

TestApp::TestApp()
{
    m_blinky_timer.set_interrupts(stm32::timer::BasicTimer::DierBits::UIE, 0);
    m_blinky_timer.enable(true);
}

void TestApp::run()
{
	while(1)
    {
        
    }    
}

void TestApp::blinky_timer_callback()
{
    m_blinky_led.toggle();
}