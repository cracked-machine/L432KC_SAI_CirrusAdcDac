#ifndef __advanced_timer_hpp__
#define __advanced_timer_hpp__

#include <general_timer_16bit.hpp>

namespace stm32::timer
{

/// @brief Advanced Timer
/// @note This inerits from GeneralTimer16Bit (rather than BasicTimer) so that it can share 
/// "TIM1_BRK_TIM15_IRQHandler()" and "TIM1_UP_TIM16_IRQHandler()" with GeneralTimer16Bit
class AdvancedTimer : public GeneralTimer16Bit
{
public:

    /// @brief The timer peripheral block for AdvancedTimer
    enum class Block
    {
        T1 = 1,
    };

    /// @brief Status Register bit absractions for AdvancedTimer
    enum class StatusBits
    {
        UIF     = TIM_SR_UIF,
        CC1IF   = TIM_SR_CC1IF,
        CC2IF   = TIM_SR_CC2IF,
        CC3IF   = TIM_SR_CC3IF,
        CC4IF   = TIM_SR_CC4IF,
        COMIF   = TIM_SR_COMIF,
        TIF     = TIM_SR_TIF,
        BIF     = TIM_SR_BIF,
        B2IF    = TIM_SR_B2IF,
        CC1OF   = TIM_SR_CC1OF,
        CC2OF   = TIM_SR_CC2OF,
        CC3OF   = TIM_SR_CC3OF,
        CC4OF   = TIM_SR_CC4OF,
        SBIF    = TIM_SR_SBIF
    };

    /// @brief DMA/Interrupt Enable Register bit absractions for AdvancedTimer
    enum class DierBits
    {
        UIE     = TIM_DIER_UIE,
        CC1IE   = TIM_DIER_CC1IE,
        CC2IE   = TIM_DIER_CC2IE,
        CC3IE   = TIM_DIER_CC3IE,
        CC4IE   = TIM_DIER_CC4IE,
        COMIE   = TIM_DIER_COMIE,
        TIE     = TIM_DIER_TIE,
        BIE     = TIM_DIER_BIE,
        UDE     = TIM_DIER_UDE,
        CC1DE   = TIM_DIER_CC1DE,
        CC2DE   = TIM_DIER_CC2DE,
        CC3DE   = TIM_DIER_CC3DE,
        CC4DE   = TIM_DIER_CC4DE,
        COMDE   = TIM_DIER_COMDE,
        TDE     = TIM_DIER_TDE            
    };    

    /// @brief enumeration of BasicTimer interrupt vector abstractions
    enum class ISRVectors
    {
        TIM1_BRK_TIM15_IRQHandler,
        TIM1_UP_TIM16_IRQHandler,
        TIM1_TRG_COM_IRQHandler,
        TIM1_CC_IRQHandler,
        LENGTH
    };    

    AdvancedTimer(Block timer_block, uint16_t psc, uint16_t arr, uint16_t cnt = 0, bool delayed_start = false);

    /// @brief Setup the interrupt for this timer.
    /// @param dier The interrupt to enable
    /// @param vector The interrupt vector to use
    /// @param handler The object to set as the target for callback to ISR() function
    /// @param prio The interrupt priority (numerically lower is higher priority)
    /// @return [[nodiscard]] True if setup was successful, false if not 
    [[nodiscard]] bool set_interrupts(AdvancedTimer::DierBits dier, 
                        AdvancedTimer::ISRVectors vector, 
                        AdvancedTimer* handler, 
                        uint32_t prio);

    /// @brief Array of mappings between ISRs and callback pointers when ISRs are captured
    static inline std::array<AdvancedTimer*, static_cast<std::size_t>(ISRVectors::LENGTH)> m_adv_timer_irq_mappings;

private:

    /// @brief Low level function for mapping between ISRs and callback pointers.
    /// @param vector 
    /// @param handler 
    void register_handler_base(AdvancedTimer::ISRVectors vector, AdvancedTimer* handler);

};

extern "C" void TIM1_TRG_COM_IRQHandler(void);
extern "C" void TIM1_CC_IRQHandler(void);  

} // namespace stm32::timer

#endif // __advanced_timer_hpp__