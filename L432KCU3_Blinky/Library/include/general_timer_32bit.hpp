#ifndef __general_timer_32bit_hpp__
#define __general_timer_32bit_hpp__

#include <basic_timer.hpp>

namespace stm32::timer
{

class GeneralTimer32Bit : public BasicTimer
{
public:

    /// @brief The timer peripheral block for GeneralTimer32Bit
    enum class Block
    {
        T2 = 2
    };

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

    /// @brief enumeration of BasicTimer interrupt vector abstractions
    enum class ISRVectors
    {
        TIM2_IRQHandler,
        LENGTH
    };    

    GeneralTimer32Bit(Block timer_block, uint16_t psc, uint16_t arr, uint16_t cnt = 0, bool delayed_start = false);

    /// @brief Setup the interrupt for this timer.
    /// @param dier The interrupt to enable
    /// @param vector The interrupt vector to use
    /// @param handler The object to set as the target for callback to ISR() function
    /// @param prio The interrupt priority (numerically lower is higher priority)
    /// @return [[nodiscard]] True if setup was successful, false if not 
    [[nodiscard]] bool set_interrupts(GeneralTimer32Bit::DierBits dier, 
                        GeneralTimer32Bit::ISRVectors vector,
                        GeneralTimer32Bit* handler,
                        uint32_t prio);

    /// @brief Array of mappings between ISRs and callback pointers when ISRs are captured
    static inline std::array<GeneralTimer32Bit*, static_cast<std::size_t>(ISRVectors::LENGTH)> m_gen_timer32_irq_mappings;

private:

    /// @brief Low level function for mapping between ISRs and callback pointers.
    /// @param vector 
    /// @param handler 
    void register_handler_base(GeneralTimer32Bit::ISRVectors vector, GeneralTimer32Bit* handler);

};

extern "C" void TIM2_IRQHandler(void);



} // namespace stm32::timer

#endif // __general_timer_32bit_hpp__