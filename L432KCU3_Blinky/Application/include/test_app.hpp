#ifndef __test_app_hpp__
#define __test_app_hpp__

#include <system_device.hpp>


class TestApp
{
public:
    TestApp() = default;
    void run();
private:

    stm32::SystemDevice m_system;

    stm32::gpio::Driver m_blinky_led{
        stm32::gpio::Port::B, 
        stm32::gpio::Pin::PIN3, 
        stm32::gpio::Mode::OUTPUT, 
        stm32::gpio::Otype::PUSHPULL, 
        stm32::gpio::Speed::VHIGH,
        stm32::gpio::Pupd::PU
    };    

};

#endif // __test_app_hpp__