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
        BlinkyTimer1(BasicTimer::ISRVectors irq, TestApp* handler) 
        : stm32::timer::BasicTimer(irq, 1, 1, 0, true), m_irq(irq), m_handler(handler)         
        {
            using namespace stm32::timer;            
            [[maybe_unused]] bool result = set_interrupts(BasicTimer::DierBits::UIE, m_irq, this, 1);
            enable(true);
        }
    private:
        /// @brief NVIC IRQ instance to use for this interrupt
        BasicTimer::ISRVectors m_irq;
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
        BlinkyTimer2(GeneralTimer16Bit::ISRVectors irq, TestApp* handler) 
        : stm32::timer::GeneralTimer16Bit(irq, 16, 1024, 0, true), m_irq(irq), m_handler(handler)          
        {
            using namespace stm32::timer;            
            [[maybe_unused]] bool result = set_interrupts(GeneralTimer16Bit::DierBits::UIE, m_irq, this, 0);
            enable(true);
        }
    private:
        /// @brief NVIC IRQ instance to use for this interrupt
        GeneralTimer16Bit::ISRVectors m_irq;
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
        BlinkyTimer3(GeneralTimer32Bit::ISRVectors irq, TestApp* handler) 
        : stm32::timer::GeneralTimer32Bit(irq, 16, 1024, 0, true), m_irq(irq), m_handler(handler)       
        {
            using namespace stm32::timer;         
            [[maybe_unused]] bool result = set_interrupts(GeneralTimer32Bit::DierBits::UIE, m_irq, this, 0);
            enable(true);
        }
    private:
        GeneralTimer32Bit::ISRVectors m_irq = GeneralTimer32Bit::ISRVectors::TIM2_IRQHandler;
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
            m_handler = handler;       
            
            [[maybe_unused]] bool result = set_interrupts(AdvancedTimer::DierBits::UIE, m_isr_vector, this, 0);
            enable(true);
        }
    private:
        AdvancedTimer::ISRVectors m_isr_vector = AdvancedTimer::ISRVectors::TIM1_UP_TIM16_IRQHandler;
        /// @brief Pointer to the parent class
        TestApp* m_handler = nullptr;
        /// @brief Redirect the ISR() callback to parent class non-static member function 
        void ISR() override { m_handler->blinky_timer_callback(); }
    };    

    
    /// @brief led blink timer object 
    // BlinkyTimer1 m_blinky_timer{stm32::timer::BasicTimer::ISRVectors::TIM7_IRQHandler, this};
    // BlinkyTimer2 m_blinky_timer{stm32::timer::GeneralTimer16Bit::ISRVectors::TIM1_BRK_TIM15_IRQHandler, this};
    BlinkyTimer3 m_blinky_timer{stm32::timer::GeneralTimer32Bit::ISRVectors::TIM2_IRQHandler, this};

};

#endif // __test_app_hpp__