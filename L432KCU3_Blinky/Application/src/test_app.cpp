#include <test_app.hpp>

void TestApp::run()
{

    /* Loop forever */
	while(1)
    {

        // delay_ms(100);
        m_blinky_led.toggle();

    }    
}