#ifndef __basic_timer_hpp__
#define __basic_timer_hpp__

#if defined (USE_HAL_DRIVER)
    #undef USE_HAL_DRIVER
#endif
#include <stm32l4xx.h>
#include <array>
namespace stm32::timer
{

class BasicTimer
{
public:

    /// @brief The timer peripheral block for BasicTimer
    enum class Block
    {
        T6 = 6,
        T7 = 7
    };
  
    /// @brief Status Register bit absractions for BasicTimer
    enum class StatusBits
    {
        UIF = TIM_SR_UIF
    };

    /// @brief DMA/Interrupt Enable Register bit absractions for BasicTimer
    enum class DierBits
    {
        UDE = TIM_DIER_UDE,
        UIE = TIM_DIER_UIE
    };

    /// @brief enumeration of BasicTimer interrupt vector abstractions
    enum class ISRVectors
    {
        TIM6_DACUNDER_IRQHandler,
        TIM7_IRQHandler,
        LENGTH
    };    

    /// @brief Construct a new Basic Timer object. 
    /// Implicit defaults are: one pulse mode disabled, all UEV source enabled, auto-reload preload enabled
    /// @param irq The timer/IRQ to use
    /// @param psc The prescaler value
    /// @param arr The auto-reload value
    /// @param cnt The initial counter value
    /// @param delayed_start if true enable timer now, if false leave disabled. True by default
    BasicTimer(ISRVectors irq, uint16_t psc, uint16_t arr, uint16_t cnt, bool delayed_start);
    
    void enable(bool enable_timer);

    /// @brief Set the "one pulse mode"
    /// @param opm If true set "one pulse mode", false by default.
    void set_opm(bool opm = false);

    /// @brief Set the "Update Request Source"
    /// @param urs_cnt_only If true limit update interrupt/DMA request to counter overflow/underflow only, 
    /// false by default.
    void set_urs(bool urs_cnt_only = false);

    /// @brief Set the "Auto-reload Preload"
    /// @param arpe If false TIMx_ARR register is not buffered, true by default.
    void set_arpe(bool enable_arpe = true);

    /// @brief Set the "Update Disable"
    /// @param enable_uev if true update event (uev) is generated by counter overflow/undeflow, 
    /// setting UG bit or update via slave controller. True by default.
    void set_udis(bool enable_uev = true);

    void set_psc(uint16_t psc);
    void set_arr(uint16_t arr);
    void set_cnt(uint16_t cnt);

    /// @brief Setup the interrupt for this timer.
    /// @param dier The interrupt to enable
    /// @param vector The interrupt vector to use
    /// @param handler The object to set as the target for callback to ISR() function
    /// @param prio The interrupt priority (numerically lower is higher priority)
    /// @return [[nodiscard]] True if setup was successful, false if not. 
    [[nodiscard]] bool set_interrupts(  BasicTimer::DierBits dier, 
                                        BasicTimer::ISRVectors vector, 
                                        BasicTimer* handler, 
                                        uint32_t prio);

    /// @brief Check the "Status Register" bit. 
    /// @param sr of type BasicTimer::StatusBits
    /// @param auto_clear if true the bit is cleared on read. True by default.
    /// @return true if bit is set, false it not
    bool is_status_set(BasicTimer::StatusBits sr, bool auto_clear = true);

    /// @brief Array of mappings between ISRs and callback pointers when ISRs are captured
    static inline std::array<BasicTimer*, static_cast<std::size_t>(ISRVectors::LENGTH)> m_basic_timer_irq_mappings;

    /// @brief Placeholder for the function called by the low level ISR. Should be defined by derived class.
    virtual void ISR() = 0;

protected:

    BasicTimer(Block timer_block);

    void init_cr1(uint16_t psc, uint16_t arr, uint16_t cnt = 0, bool delayed_start = false);

    /// @brief Structure holding the timer peripheral registers
    TIM_TypeDef* m_timer_registers;
    /// @brief Abstracted TIM peripheral, TIM1, TIM2, TIM3, etc...
    Block m_timer_block;

private:
    /// @brief Low level function for mapping between ISRs and callback pointers.
    /// @param vector 
    /// @param handler 
    void register_handler_base(BasicTimer::ISRVectors vector, BasicTimer* handler);
};

extern "C" void TIM6_DACUNDER_IRQHandler(void);
extern "C" void TIM7_IRQHandler(void);


} // namespace stm32::timer


#endif // __basic_timer_hpp__