#include <advanced_timer.hpp>

namespace stm32::timer
{

AdvancedTimer::AdvancedTimer(AdvancedTimer::Block timer_block, uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start)
: GeneralTimer16Bit(static_cast<GeneralTimer16Bit::Block>(timer_block))
{
    // Each case will: 
    // 1) assign the requested TIM_TypeDef instance to the m_timer_registers member 
    // 2) Enable the requested APB1 bus clock for that timer block. Read back enforces a required short delay.
    [[maybe_unused]]  __IO uint32_t tmpreg;     
    switch(timer_block)
    {
        case AdvancedTimer::Block::T1:
            m_timer_registers = TIM1;
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN); 
            tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);        
            break;
    }

    // call base member for common init
    init_cr1(psc, arr, cnt, delayed_start);
}

void AdvancedTimer::set_interrupts(AdvancedTimer::DierBits dier, uint32_t prio)
{
    // Setup the NVIC, priority and DIER register
    switch(static_cast<AdvancedTimer::Block> (m_timer_block))
    {
        case Block::T1:
            NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, prio);
            NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); 

            NVIC_SetPriority(TIM1_UP_TIM16_IRQn, prio);
            NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);   

            NVIC_SetPriority(TIM1_TRG_COM_IRQn, prio);
            NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);  

            NVIC_SetPriority(TIM1_CC_IRQn, prio);
            NVIC_EnableIRQ(TIM1_CC_IRQn);                                                 
            break;
    }    
    m_timer_registers->DIER = static_cast<uint16_t>(dier);
}

void AdvancedTimer::register_handler_base(AdvancedTimer::ISRVectors vector, AdvancedTimer* handler)
{
    switch(vector)
    {
        // We can't re-define "TIM1_BRK_TIM15_IRQHandler()" and "TIM1_UP_TIM16_IRQHandler()" here
        // so assign these handlers to "m_gen_timer16_irq_mappings"
        case ISRVectors::TIM1_BRK_TIM15_IRQHandler:
        case ISRVectors::TIM1_UP_TIM16_IRQHandler:
            m_gen_timer16_irq_mappings[static_cast<std::size_t>(vector)] = handler;
            break;

        case ISRVectors::TIM1_CC_IRQHandler:
        case ISRVectors::TIM1_TRG_COM_IRQHandler:
            m_adv_timer_irq_mappings[static_cast<std::size_t>(vector)] = handler;
            break;
    }
    
    
}    

extern "C" void TIM1_TRG_COM_IRQHandler(void)
{
    using namespace stm32::timer;
    AdvancedTimer::m_adv_timer_irq_mappings[static_cast<std::size_t>(AdvancedTimer::ISRVectors::TIM1_TRG_COM_IRQHandler)]->ISR();
    TIM1->SR &= ~TIM_SR_UIF;
}

extern "C" void TIM1_CC_IRQHandler(void)
{
    using namespace stm32::timer;
    AdvancedTimer::m_adv_timer_irq_mappings[static_cast<std::size_t>(AdvancedTimer::ISRVectors::TIM1_CC_IRQHandler)]->ISR();
    TIM1->SR &= ~TIM_SR_UIF;
}



} // namespace stm32::timer