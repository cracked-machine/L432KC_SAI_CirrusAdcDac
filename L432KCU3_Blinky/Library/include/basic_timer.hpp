#ifndef __basic_timer_hpp__
#define __basic_timer_hpp__

#if defined (USE_HAL_DRIVER)
    #undef USE_HAL_DRIVER
#endif
#include <stm32l4xx.h>

namespace stm32::timer
{

enum class Block
{
    T6,
    T7
};

class BasicTimer
{
public:
    BasicTimer(Block timer_id);

private:
    TIM_TypeDef* m_timer_registers;

};


} // namespace stm32::timer


#endif // __basic_timer_hpp__