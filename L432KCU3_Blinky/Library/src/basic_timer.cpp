#include <basic_timer.hpp>

namespace stm32::timer
{


BasicTimer::BasicTimer(Block timer_id)
{
    // Each case will: 
    // 1) assign the requested TIM_TypeDef instance to the m_timer_registers member 
    // 2) Enable the requested APB1 bus clock for that timer block. Read back enforces a required short delay.
    [[maybe_unused]]  __IO uint32_t tmpreg;     
    switch(timer_id)
    {
        case Block::T6:
            m_timer_registers = TIM6;
            SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN); 
            tmpreg = READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN);                     
            break;
        case Block::T7:
            m_timer_registers = TIM7;
            SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN); 
            tmpreg = READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN);  
            break;
    }
}


} // namespace stm32::timer