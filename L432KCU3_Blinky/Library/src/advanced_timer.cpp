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

bool AdvancedTimer::set_interrupts( AdvancedTimer::DierBits dier, 
                                    AdvancedTimer::ISRVectors vector, 
                                    AdvancedTimer* handler, 
                                    uint32_t prio)
{
    switch(dier)
    {
        case DierBits::UDE:
            // should not set DMA using this function
            return false;

        case DierBits::CC1IE:
        case DierBits::CC2IE:
        case DierBits::CC3IE:
        case DierBits::CC4IE:
        case DierBits::COMIE:
        case DierBits::TIE:
        case DierBits::BIE:
        case DierBits::CC1DE:
        case DierBits::CC2DE:
        case DierBits::CC3DE:
        case DierBits::CC4DE:
        case DierBits::COMDE:
        case DierBits::TDE:
            // not implemented 
            return false;                
        
        case DierBits::UIE:
            // Set the caller "handler" as the target for callback to ISR() function
            register_handler_base(vector, handler);          

            // Setup the NVIC, priority and DIER register
            switch(vector)
            {
                case ISRVectors::TIM1_BRK_TIM15_IRQHandler:
                    NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, prio);
                    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); 
                    break;
                case ISRVectors::TIM1_UP_TIM16_IRQHandler:
                    NVIC_SetPriority(TIM1_UP_TIM16_IRQn, prio);
                    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);   
                    break;
                case ISRVectors::TIM1_TRG_COM_IRQHandler:
                    NVIC_SetPriority(TIM1_TRG_COM_IRQn, prio);
                    NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);  
                    break;
                case ISRVectors::TIM1_CC_IRQHandler:
                    NVIC_SetPriority(TIM1_CC_IRQn, prio);
                    NVIC_EnableIRQ(TIM1_CC_IRQn);                                                 
                    break;
            }    
            m_timer_registers->DIER = static_cast<uint16_t>(dier);

        default:
            return false;
    }
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