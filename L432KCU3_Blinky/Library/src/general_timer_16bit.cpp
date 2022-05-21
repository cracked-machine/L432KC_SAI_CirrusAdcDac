#include <general_timer_16bit.hpp>

namespace stm32::timer
{

GeneralTimer16Bit::GeneralTimer16Bit(GeneralTimer16Bit::Block timer_block, uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start)
: BasicTimer(static_cast<BasicTimer::Block>(timer_block))
{
    // Each case will: 
    // 1) assign the requested TIM_TypeDef instance to the m_timer_registers member 
    // 2) Enable the requested APB1 bus clock for that timer block. Read back enforces a required short delay.
    [[maybe_unused]]  __IO uint32_t tmpreg;     
    switch(timer_block)
    {
        case GeneralTimer16Bit::Block::T15:
            m_timer_registers = TIM15;
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN); 
            tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN);        
            break;
        
        case GeneralTimer16Bit::Block::T16:
            m_timer_registers = TIM16;
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN); 
            tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN);  
            break;
    }

    // call base member for common init
    init_cr1(psc, arr, cnt, delayed_start);
}

GeneralTimer16Bit::GeneralTimer16Bit(Block timer_block)
: BasicTimer(static_cast<BasicTimer::Block>(timer_block))
{}

bool GeneralTimer16Bit::set_interrupts( GeneralTimer16Bit::DierBits dier,
                                        GeneralTimer16Bit::ISRVectors vector, 
                                        GeneralTimer16Bit* handler, 
                                        uint32_t prio)
{
    switch(dier)
    {
        case DierBits::UDE:
            // should not set DMA using this function
            return false;

        case DierBits::CC1DE:
        case DierBits::CC1IE:
        case DierBits::COMDE:
        case DierBits::COMIE:
        case DierBits::BIE:
            // not implemented 
            return false;

        case DierBits::UIE:
            // Set the caller as the target for callback to ISR() function
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
            }    
            m_timer_registers->DIER = static_cast<uint16_t>(dier);            
            return true;

        default: 
            return false;
    }

}

void GeneralTimer16Bit::register_handler_base(GeneralTimer16Bit::ISRVectors vector, GeneralTimer16Bit* handler)
{
    m_gen_timer16_irq_mappings[static_cast<std::size_t>(vector)] = handler;
}    


extern "C" void TIM1_BRK_TIM15_IRQHandler(void)
{
    using namespace stm32::timer;
    GeneralTimer16Bit::m_gen_timer16_irq_mappings[static_cast<std::size_t>(GeneralTimer16Bit::ISRVectors::TIM1_BRK_TIM15_IRQHandler)]->ISR();
    TIM15->SR &= ~TIM_SR_UIF;
}

extern "C" void TIM1_UP_TIM16_IRQHandler(void)
{
    using namespace stm32::timer;
    GeneralTimer16Bit::m_gen_timer16_irq_mappings[static_cast<std::size_t>(GeneralTimer16Bit::ISRVectors::TIM1_UP_TIM16_IRQHandler)]->ISR();
    TIM16->SR &= ~TIM_SR_UIF;
}

} // namespace stm32::timer