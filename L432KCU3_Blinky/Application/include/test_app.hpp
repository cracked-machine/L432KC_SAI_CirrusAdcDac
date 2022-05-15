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
    // system configuration 
    stm32::SystemDevice m_system;

    // GPIO for an LED
    stm32::gpio::Driver m_blinky_led
    {
        stm32::gpio::Port::B, 
        stm32::gpio::Pin::PIN3, 
        stm32::gpio::Mode::OUTPUT, 
        stm32::gpio::Otype::PUSHPULL, 
        stm32::gpio::Speed::VHIGH,
        stm32::gpio::Pupd::PU
    };    

    /// @brief Inner class timer for an LED
    class BlinkyTimer1 : public stm32::timer::BasicTimer
    {
    public:
        /// @brief Construct a new BlinkyTimer object
        /// @param handler Pointer to the parent class
        BlinkyTimer1(TestApp* handler) 
        : stm32::timer::BasicTimer(stm32::timer::BasicTimer::Block::T7, 1, 1, 0, true)         
        {
            using namespace stm32::timer;

            // register the parent class pointer with ISR() callback
            m_handler = handler;
            BasicTimer::register_handler_base(BasicTimer::ISRVectors::TIM7_IRQHandler, this);

            enable(true);
            set_interrupts(stm32::timer::BasicTimer::DierBits::UIE, 0);
        }
    private:
        /// @brief Pointer to the parent class
        TestApp* m_handler = nullptr;
        /// @brief Redirect the ISR() callback to parent class non-static member function 
        void ISR() { m_handler->blinky_timer_callback(); }
    };


    /// @brief Inner class timer for an LED
    class BlinkyTimer2 : public stm32::timer::GeneralTimer16Bit
    {
    public:
        /// @brief Construct a new BlinkyTimer object
        /// @param handler Pointer to the parent class
        BlinkyTimer2(TestApp* handler) 
        : stm32::timer::GeneralTimer16Bit(stm32::timer::GeneralTimer16Bit::Block::T15, 16, 1024, 0, true)        
        {
            using namespace stm32::timer;
            // register the parent class pointer with ISR() callback
            m_handler = handler;
            GeneralTimer16Bit::register_handler_base(GeneralTimer16Bit::ISRVectors::TIM1_BRK_TIM15_IRQHandler, this);
            
            enable(true);
            set_interrupts(stm32::timer::GeneralTimer16Bit::DierBits::UIE, 0);
        }
    private:
        /// @brief Pointer to the parent class
        TestApp* m_handler = nullptr;
        /// @brief Redirect the ISR() callback to parent class non-static member function 
        void ISR() override { m_handler->blinky_timer_callback(); }
    };

    /// @brief Inner class timer for an LED
    class BlinkyTimer3 : public stm32::timer::GeneralTimer32Bit
    {
    public:
        /// @brief Construct a new BlinkyTimer object
        /// @param handler Pointer to the parent class
        BlinkyTimer3(TestApp* handler) 
        : stm32::timer::GeneralTimer32Bit(stm32::timer::GeneralTimer32Bit::Block::T2, 16, 1024, 0, true)        
        {
            using namespace stm32::timer;
            // register the parent class pointer with ISR() callback
            m_handler = handler;
            GeneralTimer32Bit::register_handler_base(GeneralTimer32Bit::ISRVectors::TIM2_IRQHandler, this);
            
            enable(true);
            set_interrupts(stm32::timer::GeneralTimer32Bit::DierBits::UIE, 0);
        }
    private:
        /// @brief Pointer to the parent class
        TestApp* m_handler = nullptr;
        /// @brief Redirect the ISR() callback to parent class non-static member function 
        void ISR() override { m_handler->blinky_timer_callback(); }
    };

    /// @brief Inner class timer for an LED
    class BlinkyTimer4 : public stm32::timer::AdvancedTimer
    {
    public:
        /// @brief Construct a new BlinkyTimer object
        /// @param handler Pointer to the parent class
        BlinkyTimer4(TestApp* handler) 
        : stm32::timer::AdvancedTimer(stm32::timer::AdvancedTimer::Block::T1, 16, 1024, 0, true)        
        {
            using namespace stm32::timer;
            // register the parent class pointer with ISR() callback
            m_handler = handler;
            AdvancedTimer::register_handler_base(AdvancedTimer::ISRVectors::TIM1_UP_TIM16_IRQHandler, this);
            
            enable(true);
            set_interrupts(stm32::timer::AdvancedTimer::DierBits::UIE, 0);
        }
    private:
        /// @brief Pointer to the parent class
        TestApp* m_handler = nullptr;
        /// @brief Redirect the ISR() callback to parent class non-static member function 
        void ISR() override { m_handler->blinky_timer_callback(); }
    };    




    /// @brief led blink timer object 
    BlinkyTimer2 m_blinky_timer{this};

};

#endif // __test_app_hpp__