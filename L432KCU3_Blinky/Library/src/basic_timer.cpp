#include <basic_timer.hpp>

namespace stm32::timer
{

BasicTimer::BasicTimer(ISRVectors irq, uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start)
{
    // Each case will: 
    // 1) assign the requested TIM_TypeDef instance to the m_timer_registers member 
    // 2) Enable the requested APB1 bus clock for that timer block. Read back enforces a required short delay.
    [[maybe_unused]]  __IO uint32_t tmpreg;     
    switch(irq)
    {
        case ISRVectors::TIM6_DACUNDER_IRQHandler:
            m_timer_registers = TIM6;
            SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN); 
            tmpreg = READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN);        
            break;
        
        case ISRVectors::TIM7_IRQHandler:
            m_timer_registers = TIM7;
            SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN); 
            tmpreg = READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN);  
            break;
    }

    init_cr1(psc, arr, cnt, delayed_start);
}

BasicTimer::BasicTimer(Block timer_block)
: m_timer_block(timer_block)
{}

void BasicTimer::init_cr1(uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start)
{
    // set the CR1 with defaults
    set_opm();
    set_urs();
    set_arpe();
    set_udis();

    set_cnt(cnt);
    set_arr(arr);    

    // Generate an update event to reload the Prescaler immediately
    set_psc(psc);
    m_timer_registers->EGR = TIM_EGR_UG;  

    // Enable the timer
    if (!delayed_start) { enable(true); }    
}

void BasicTimer::enable(bool enable_timer)
{
    if (enable_timer) { SET_BIT(m_timer_registers->CR1, TIM_CR1_CEN); }
    else { CLEAR_BIT(m_timer_registers->CR1, TIM_CR1_CEN); }
}

void BasicTimer::set_opm(bool opm)
{
    if (opm) { SET_BIT(m_timer_registers->CR1, TIM_CR1_OPM); }
    else { CLEAR_BIT(m_timer_registers->CR1, TIM_CR1_OPM); }
}

void BasicTimer::set_urs(bool urs_cnt_only)
{
    if (urs_cnt_only) { SET_BIT(m_timer_registers->CR1, TIM_CR1_URS); }
    else { CLEAR_BIT(m_timer_registers->CR1, TIM_CR1_URS); }
}

void BasicTimer::set_arpe(bool enable_arpe)
{
    if (enable_arpe) { SET_BIT(m_timer_registers->CR1, TIM_CR1_ARPE); }
    else {CLEAR_BIT(m_timer_registers->CR1, TIM_CR1_ARPE); }
}

void BasicTimer::set_udis(bool enable_uev)
{
    if (enable_uev) { CLEAR_BIT(m_timer_registers->CR1, TIM_CR1_UDIS); }
    else { SET_BIT(m_timer_registers->CR1, TIM_CR1_UDIS); }        
}

void BasicTimer::set_psc(uint16_t psc)
{
    m_timer_registers->PSC = psc;
}

void BasicTimer::set_arr(uint16_t arr)
{
    m_timer_registers->ARR = arr;
}

void BasicTimer::set_cnt(uint16_t cnt)
{
    m_timer_registers->CNT = cnt;
}

bool BasicTimer::set_interrupts(BasicTimer::DierBits dier, 
                                BasicTimer::ISRVectors vector,
                                BasicTimer* handler, 
                                uint32_t prio)
{
    switch(dier)
    {
        case DierBits::UDE:
            // if TIM6 use DMA1 Ch3 (DMA_CSELR->C3S = 0110) or DMA2 Ch4 (DMA_CSELR->C4S = 0011)
            // if TIM7 use DMA1 Ch4 (DMA_CSELR->C4S = 0101), DMA2 Ch5 (DMA_CSELR->C5S = 0011)
            // should not set DMA using this function
            return false;
        case DierBits::UIE:
            // Set the caller as the target for callback to ISR() function
            register_handler_base(vector, handler);
            
            // Setup the NVIC, priority and DIER register
            switch(vector)
            {
                case ISRVectors::TIM6_DACUNDER_IRQHandler:
                    NVIC_SetPriority(TIM6_DAC_IRQn, prio);
                    NVIC_EnableIRQ(TIM6_DAC_IRQn);             
                    break;
                
                case ISRVectors::TIM7_IRQHandler:
                    NVIC_SetPriority(TIM7_IRQn, prio);
                    NVIC_EnableIRQ(TIM7_IRQn);             
                    break;
            }    
            m_timer_registers->DIER = static_cast<uint16_t>(dier);    
            return true;
        
        default:
            return false;
    } 
}

bool BasicTimer::is_status_set(StatusBits sr, bool auto_clear)
{
    if (m_timer_registers->SR & static_cast<uint16_t>(sr)) 
    { 
        if (auto_clear) { m_timer_registers->SR &= ~static_cast<uint16_t>(sr); }
        return true; 
    }
    else 
    { 
        return false; 
    }
}

void BasicTimer::register_handler_base(BasicTimer::ISRVectors vector, BasicTimer* handler)
{
    m_basic_timer_irq_mappings[static_cast<std::size_t>(vector)] = handler;
}

extern "C" void TIM6_DACUNDER_IRQHandler(void)
{
    using namespace stm32::timer;
    BasicTimer::m_basic_timer_irq_mappings[static_cast<std::size_t>(BasicTimer::ISRVectors::TIM6_DACUNDER_IRQHandler)]->ISR();
    TIM6->SR &= ~TIM_SR_UIF;
}

extern "C" void TIM7_IRQHandler(void)
{
    using namespace stm32::timer;
    BasicTimer::m_basic_timer_irq_mappings[static_cast<std::size_t>(BasicTimer::ISRVectors::TIM7_IRQHandler)]->ISR();
    TIM7->SR &= ~TIM_SR_UIF;
}

} // namespace stm32::timer