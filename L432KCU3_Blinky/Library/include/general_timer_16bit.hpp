#ifndef _general_timer_16bit_hpp_
#define _general_timer_16bit_hpp_

#include <basic_timer.hpp>

namespace stm32::timer
{

class GeneralTimer16Bit : public BasicTimer
{
public:

    /// @brief The timer peripheral block for GeneralTimer16Bit
    enum class Block
    {
        T15 = 15,
        T16 = 16
    };

    /// @brief Status Register bit absractions for GeneralTimer16Bit
    enum class StatusBits
    {
        UIF     = TIM_SR_UIF,
        CC1IF   = TIM_SR_CC1IF,
        COMIF   = TIM_SR_COMIF,
        BIF     = TIM_SR_BIF,
        CC1OF   = TIM_SR_CC1OF
    };

    /// @brief DMA/Interrupt Enable Register bit absractions for GeneralTimer16Bit
    enum class DierBits
    {
        UIE     = TIM_DIER_UIE,
        CC1IE   = TIM_DIER_CC1IE,
        COMIE   = TIM_DIER_COMIE,
        BIE     = TIM_DIER_BIE,
        UDE     = TIM_DIER_UDE,
        CC1DE   = TIM_DIER_CC1DE,
        COMDE   = TIM_DIER_COMDE        
    };    

    /// @brief enumeration of BasicTimer interrupt vector abstractions
    enum class ISRVectors
    {
        TIM1_BRK_TIM15_IRQHandler,
        TIM1_UP_TIM16_IRQHandler,
        LENGTH
    };    

    GeneralTimer16Bit(Block timer_block, uint16_t psc, uint16_t arr, uint16_t cnt = 0, bool delayed_start = false);

    /// @brief Setup the interrupt for this timer.
    /// @param dier The interrupt to enable
    /// @param vector The interrupt vector to use
    /// @param handler The object to set as the target for callback to ISR() function
    /// @param prio The interrupt priority (numerically lower is higher priority)
    /// @return [[nodiscard]] True if setup was successful, false if not 
    [[nodiscard]] bool set_interrupts(GeneralTimer16Bit::DierBits dier,
                        GeneralTimer16Bit::ISRVectors vector, 
                        GeneralTimer16Bit* handler, 
                        uint32_t prio);

    /// @brief Array of mappings between ISRs and callback pointers when ISRs are captured
    static inline std::array<GeneralTimer16Bit*, static_cast<std::size_t>(ISRVectors::LENGTH)> m_gen_timer16_irq_mappings;


protected:
    GeneralTimer16Bit(Block timer_block);

private:
    /// @brief Low level function for mapping between ISRs and callback pointers.
    /// @param vector 
    /// @param handler 
    void register_handler_base(GeneralTimer16Bit::ISRVectors vector, GeneralTimer16Bit* handler);
};

extern "C" void TIM1_BRK_TIM15_IRQHandler(void);
extern "C" void TIM1_UP_TIM16_IRQHandler(void);


} // namespace stm32::timer

#endif // _general_timer_16bit_hpp_