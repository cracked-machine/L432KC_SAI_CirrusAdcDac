#include <general_timer_32bit.hpp>

namespace stm32::timer
{

GeneralTimer32Bit::GeneralTimer32Bit(GeneralTimer32Bit::Block timer_block, uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start)
: BasicTimer(static_cast<BasicTimer::Block>(timer_block))
{
    // Each case will: 
    // 1) assign the requested TIM_TypeDef instance to the m_timer_registers member 
    // 2) Enable the requested APB1 bus clock for that timer block. Read back enforces a required short delay.
    [[maybe_unused]]  __IO uint32_t tmpreg;     
    switch(timer_block)
    {
        case GeneralTimer32Bit::Block::T2:
            m_timer_registers = TIM2;
            SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN); 
            tmpreg = READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN);        
            break;
    }

    // call base member for common init
    init_cr1(psc, arr, cnt, delayed_start);
}

// void GeneralTimer32Bit::set_interrupts(GeneralTimer32Bit::DierBits dier, uint32_t prio)
// {
//     // Setup the NVIC, priority and DIER register
//     switch(static_cast<GeneralTimer32Bit::Block> (m_timer_block))
//     {
//         case Block::T2:
//             NVIC_SetPriority(TIM2_IRQn, prio);
//             NVIC_EnableIRQ(TIM2_IRQn);             
//             break;
//     }    
//     m_timer_registers->DIER = static_cast<uint16_t>(dier);
// }

bool GeneralTimer32Bit::set_interrupts( GeneralTimer32Bit::DierBits dier, 
                                        GeneralTimer32Bit::ISRVectors vector,
                                        GeneralTimer32Bit* handler,
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
        case DierBits::TIE:
        case DierBits::CC1DE:
        case DierBits::CC2DE:
        case DierBits::CC3DE:
        case DierBits::CC4DE:
        case DierBits::TDE:
            // not implemented 
            return false;        
        
        case DierBits::UIE:
            // Set the caller "handler" as the target for callback to ISR() function
            register_handler_base(vector, handler);        
            // Setup the NVIC, priority and DIER register
            switch(vector)
            {
                case ISRVectors::TIM2_IRQHandler:
                    NVIC_SetPriority(TIM2_IRQn, prio);
                    NVIC_EnableIRQ(TIM2_IRQn);             
                    break;
            }    
            m_timer_registers->DIER = static_cast<uint16_t>(dier);            

        default: 
            return false;
    }
}

void GeneralTimer32Bit::register_handler_base(GeneralTimer32Bit::ISRVectors vector, GeneralTimer32Bit* handler)
{
    m_gen_timer32_irq_mappings[static_cast<std::size_t>(vector)] = handler;
}    


extern "C" void TIM2_IRQHandler(void)
{
    using namespace stm32::timer;
    GeneralTimer32Bit::m_gen_timer32_irq_mappings[static_cast<std::size_t>(GeneralTimer32Bit::ISRVectors::TIM2_IRQHandler)]->ISR();
    TIM2->SR &= ~TIM_SR_UIF;
}



} // namespace stm32::timer