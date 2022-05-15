#ifndef __test_app_hpp__
#define __test_app_hpp__

#include <system_device.hpp>


class TestApp
{
public:
    TestApp();
    void run();
private:

    stm32::SystemDevice m_system;

    stm32::gpio::Driver m_blinky_led
    {
        stm32::gpio::Port::B, 
        stm32::gpio::Pin::PIN3, 
        stm32::gpio::Mode::OUTPUT, 
        stm32::gpio::Otype::PUSHPULL, 
        stm32::gpio::Speed::VHIGH,
        stm32::gpio::Pupd::PU
    };    

    stm32::timer::BasicTimer m_blinky_timer
    {
        stm32::timer::Block::T6,
        64,     // psc
        1024,   // arr
        0,      // cnt
        true    // delayed start
    };

};

#endif // __test_app_hpp__