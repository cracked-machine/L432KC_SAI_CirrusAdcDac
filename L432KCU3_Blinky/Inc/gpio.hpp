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

    Driver( GPIO_TypeDef* gpio_port, 
            Pin gpio_pin,
            Mode mode,
            Otype otype,
            Speed speed,
            Pupd pupd);        

    void set_mode(Mode mode);
    void set_otype(Otype otype);
    void set_speed(Speed speed);
    void set_pupd(Pupd pupd);
    void toggle();
    void set_pin();
    void reset_pin();

private:
    GPIO_TypeDef* m_gpio_port;
    Pin m_gpio_pin;
    uint16_t _pin_shifted = 0x1UL << static_cast<uint16_t>(m_gpio_pin);

};

} // namespace stm32

#endif // __gpio_hpp__