#include <basic_timer.hpp>

namespace stm32::timer
{

BasicTimer::BasicTimer(Block timer_id, uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start)
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

    // set the CR1 with defaults
    set_opm();
    set_urs();
    set_arpe();
    set_udis();
    
    // set the timer values
    set_psc(psc);
    set_arr(arr);
    set_cnt(cnt);
    

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
        if (enable_uev) { SET_BIT(m_timer_registers->CR1, TIM_CR1_UDIS); }
        else {CLEAR_BIT(m_timer_registers->CR1, TIM_CR1_UDIS); }        
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

    void BasicTimer::set_interrupts(DierBits dier)
    {
        m_timer_registers->DIER = static_cast<uint16_t>(dier);
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

} // namespace stm32::timer