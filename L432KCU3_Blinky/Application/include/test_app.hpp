#ifndef __test_app_hpp__
#define __test_app_hpp__

#include <system_device.hpp>


class TestApp
{
public:
    /// @brief Construct a new Test App object
    TestApp();
    /// @brief Main loop
    void run();
    /// @brief Redirected callback from static TIM IRQ
    void blinky_timer_callback();
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

    /// @brief led blink timer
    class BlinkyTimer : public stm32::timer::BasicTimer
    {
    public:
        /// @brief Construct a new BlinkyTimer object
        /// @param handler Pointer to the parent class
        BlinkyTimer(TestApp* handler) 
        : stm32::timer::BasicTimer(
            stm32::timer::Block::T6,
            1,      // psc
            1,      // arr
            0,      // cnt
            true)    // delayed start
        
        {
            // register the parent class pointer with ISR() callback
            m_handler = handler;
            stm32::timer::BasicTimer::register_handler_base(
                stm32::timer::BasicTimer::ISRVectors::TIM6_DACUNDER_IRQHandler, 
                this);
        }
    private:
        /// @brief Pointer to the parent class
        TestApp* m_handler = nullptr;
        /// @brief Redirect the ISR() callback to parent class non-static member function 
        void ISR() { m_handler->blinky_timer_callback(); }
    };

    /// @brief led blink timer object 
    BlinkyTimer m_blinky_timer{this};

};

#endif // __test_app_hpp__