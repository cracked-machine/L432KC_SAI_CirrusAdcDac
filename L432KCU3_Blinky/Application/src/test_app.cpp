#include <test_app.hpp>



TestApp::TestApp()
{

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