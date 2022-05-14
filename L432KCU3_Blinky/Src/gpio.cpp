#include <gpio.hpp>
#include <reg_macros.h>

namespace stm32::gpio {


Driver::Driver(
    Port gpio_port, 
    Pin gpio_pin,
    Mode mode,
    Otype otype,
    Speed speed,
    Pupd pupd)
    :   m_gpio_pin(gpio_pin)
{

    enable_portb(gpio_port);
    set_mode(mode);
    set_otype(otype);
    set_speed(speed);
    set_pupd(pupd);
}

void Driver::enable_port(Port gpio_port)
{
    [[maybe_unused]]  __IO uint32_t tmpreg; 
    switch(gpio_port)
    {
        case Port::A:
            m_gpio_port = GPIOA;
            SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN); 
            tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);         
            break;
        case Port::B:
            m_gpio_port = GPIOB;
            SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN); 
            tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);         
            break;
        case Port::C:
            m_gpio_port = GPIOC;
            SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN); 
            tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);         
            break;
        case Port::H:
            m_gpio_port = GPIOH;
            SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOHEN); 
            tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOHEN);         
            break;
    }

}


void Driver::set_mode(Mode mode)
{
    uint16_t _mode = static_cast<uint16_t>(mode);
    MODIFY_REG(m_gpio_port->MODER, (GPIO_MODER_MODE0 << (POSITION_VAL(_pin_shifted) * 2U)), (_mode << (POSITION_VAL(_pin_shifted) * 2U)));    
}

void Driver::set_otype(Otype otype)
{
    uint16_t _otype = static_cast<uint16_t>(otype);
    MODIFY_REG(m_gpio_port->OTYPER, _pin_shifted, (_pin_shifted * _otype));
}

void Driver::set_speed(Speed speed)
{
    uint16_t _speed = static_cast<uint16_t>(speed);
    MODIFY_REG(m_gpio_port->OSPEEDR, (GPIO_OSPEEDR_OSPEED0 << (POSITION_VAL(_pin_shifted) * 2U)), (_speed << (POSITION_VAL(_pin_shifted) * 2U)));    
}

void Driver::set_pupd(Pupd pupd)
{
    uint16_t _pupd = static_cast<uint16_t>(pupd);
    MODIFY_REG(m_gpio_port->PUPDR, (GPIO_PUPDR_PUPD0 << (POSITION_VAL(_pin_shifted) * 2U)), (_pupd << (POSITION_VAL(_pin_shifted) * 2U)));    
}

void Driver::toggle()
{

    WRITE_REG(m_gpio_port->BSRR, ((m_gpio_port->ODR & _pin_shifted) << 16u) | (~m_gpio_port->ODR & _pin_shifted));     
}

void Driver::set_pin()
{
    WRITE_REG(m_gpio_port->BSRR, _pin_shifted);    
}

void Driver::reset_pin()
{
    WRITE_REG(m_gpio_port->BRR, _pin_shifted);
}

} // namespace stm32