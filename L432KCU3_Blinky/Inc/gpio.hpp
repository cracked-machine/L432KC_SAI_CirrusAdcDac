#ifndef __gpio_hpp__
#define __gpio_hpp__


#if defined (USE_HAL_DRIVER)
    #undef USE_HAL_DRIVER
#endif
#include <stm32l4xx.h>

namespace stm32::gpio {


enum class Pin
{
    PIN0 = 0U,
    PIN1 = 1U,
    PIN2 = 2U,
    PIN3 = 3U,
    PIN4 = 4U,
    PIN5 = 5U,
    PIN6 = 6U,
    PIN7 = 7U,
    PIN8 = 8U,
    PIN9 = 9U,
    PIN10 = 10U,
    PIN11 = 11U,
    PIN12 = 12U,
    PIN13 = 13U,
    PIN14 = 14U,
    PIN15 = 15U
};

enum class Mode
{
    INPUT =     0x0UL,
    OUTPUT =    0x1UL,
    ALTFUNC =   0x2UL,
    ANALOG =    0x3UL
};

enum class Otype
{
    PUSHPULL =  0x0UL,
    OPENDRAIN = 0x1UL
};

enum class Speed
{
    LOW =       0x0UL,
    MEDIUM =    0x1UL, 
    HIGH =      0x2UL,
    VHIGH =     0x3UL
};

enum class Pupd
{
    NONE =      0x0UL,
    PU =        0x1UL,
    PD =        0x2UL
};

/// @brief GPIO Driver. Register settings must be passed to c'tor, but mutators can be called afterwards to modify their value
class Driver
{
public:
    /// @brief Construct a new gpio driver
    /// @param gpio_port The gpio port (GPIOA, GPIOB, GPIOC, etc..)
    /// @param gpio_pin The gpio pin (PIN0...PIN15)
    /// @param mode The gpio mode (output, input, analog, alternate function)
    /// @param otype The gpio type (pushpull/opendrain)
    /// @param speed The gpio speed (low/medium/high/very high)
    /// @param pupd The gpio pullup/pulldown setting
    Driver( GPIO_TypeDef* gpio_port, 
            Pin gpio_pin,
            Mode mode,
            Otype otype,
            Speed speed,
            Pupd pupd);        

    /// @brief Set the mode
    /// @param mode The gpio mode (output, input, analog, alternate function)
    void set_mode(Mode mode);

    /// @brief Set the type
    /// @param otype The gpio type (pushpull/opendrain)
    void set_otype(Otype otype);

    /// @brief Set the speed
    /// @param speed The gpio speed (low/medium/high/very high)
    void set_speed(Speed speed);

    /// @brief Set the pupd
    /// @param pupd The gpio pullup/pulldown setting
    void set_pupd(Pupd pupd);

    /// @brief Toggle the pin value
    void toggle();

    /// @brief Set the pin to 1
    void set_pin();

    /// @brief Reset the pin 0
    void reset_pin();

private:
    /// @brief The GPIO register object (port)
    GPIO_TypeDef* m_gpio_port;
    /// @brief The GPIO pin object
    Pin m_gpio_pin;
    /// @brief Convenience member; 0x1UL bitwise-shifted right by pin number
    uint16_t _pin_shifted = 0x1UL << static_cast<uint16_t>(m_gpio_pin);
};

} // namespace stm32

#endif // __gpio_hpp__