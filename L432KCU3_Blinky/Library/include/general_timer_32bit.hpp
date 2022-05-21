#ifndef __general_timer_32bit_hpp__
#define __general_timer_32bit_hpp__

#include <basic_timer.hpp>

namespace stm32::timer
{

class GeneralTimer32Bit : public BasicTimer
{
public:

    /// @brief Status Register bit absractions for GeneralTimer32Bit
    enum class StatusBits
    {
        UIF     = TIM_SR_UIF,
        CC1IF   = TIM_SR_CC1IF,
        CC2IF   = TIM_SR_CC2IF,
        CC3IF   = TIM_SR_CC3IF,
        CC4IF   = TIM_SR_CC4IF,
        TIF     = TIM_SR_TIF,
        CC1OF   = TIM_SR_CC1OF,
        CC2OF   = TIM_SR_CC2OF,
        CC3OF   = TIM_SR_CC3OF,
        CC4OF   = TIM_SR_CC4OF
    };

    /// @brief DMA/Interrupt Enable Register bit absractions for GeneralTimer32Bit
    enum class DierBits
    {
        UIE     = TIM_DIER_UIE,
        CC1IE   = TIM_DIER_CC1IE,
        CC2IE   = TIM_DIER_CC2IE,
        CC3IE   = TIM_DIER_CC3IE,
        CC4IE   = TIM_DIER_CC4IE,
        TIE     = TIM_DIER_TIE,
        UDE     = TIM_DIER_UDE,
        CC1DE   = TIM_DIER_CC1DE,
        CC2DE   = TIM_DIER_CC2DE,
        CC3DE   = TIM_DIER_CC3DE,
        CC4DE   = TIM_DIER_CC4DE,
        TDE     = TIM_DIER_TDE               
    };    

    /// @brief enumeration of BasicTimer interrupt irq abstractions
    enum class IRQs
    {
        TIM2_IRQHandler,
        LENGTH
    };    

    /// @brief Construct a new GeneralTimer32Bit object. 
    /// Implicit defaults are: one pulse mode disabled, all UEV source enabled, auto-reload preload enabled
    /// @param irq The timer/IRQ to use
    /// @param psc The prescaler value
    /// @param arr The auto-reload value
    /// @param cnt The initial counter value
    /// @param delayed_start if true enable timer now, if false leave disabled. True by default
    GeneralTimer32Bit(IRQs irq, uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start);

    /// @brief Setup the interrupt for this timer.
    /// @param dier The interrupt to enable
    /// @param irq The interrupt irq to use
    /// @param handler The object to set as the target for callback to ISR() function
    /// @param prio The interrupt priority (numerically lower is higher priority)
    /// @return [[nodiscard]] True if setup was successful, false if not 
    [[nodiscard]] bool set_interrupts(  GeneralTimer32Bit::DierBits dier, 
                                        GeneralTimer32Bit::IRQs irq,
                                        GeneralTimer32Bit* handler,
                                        uint32_t prio);

    /// @brief Array of mappings between ISRs and callback pointers when ISRs are captured
    static inline std::array<GeneralTimer32Bit*, static_cast<std::size_t>(IRQs::LENGTH)> m_gen_timer32_irq_mappings;

private:

    /// @brief Low level function for mapping between ISRs and callback pointers.
    /// @param irq 
    /// @param handler 
    void register_handler_base(GeneralTimer32Bit::IRQs irq, GeneralTimer32Bit* handler);

};

extern "C" void TIM2_IRQHandler(void);



} // namespace stm32::timer

#endif // __general_timer_32bit_hpp__