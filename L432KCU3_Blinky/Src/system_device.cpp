#include <system_device.hpp>

/// @brief The system clock tick
volatile uint32_t g_systick {0};

SystemDevice::SystemDevice()
{

    // Configure the SysTick to have interrupt in 1ms time basis
    uint32_t tick_freq_1khz = 1U;
    SysTick_Config(SystemCoreClock / (1000U / tick_freq_1khz));

    // Enable GPIO Clocks
    do { 
        __IO uint32_t tmpreg; 
        SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN); 
        /* Delay after an RCC peripheral clock enabling */ 
        tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN); 
        UNUSED(tmpreg); 
    } while(0);

    // Configure PB3 GPIO:
    // output mode
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE3_1);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE3_0);
    // push-pull
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT3);
    // set pullup
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD3_1);
    SET_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD3_0);
   
}

void SystemDevice::run()
{
    /* Loop forever */
	while(1)
    {

        delay_ms(200);
        
        uint16_t GPIO_PIN_3 = (uint16_t)0x0008;
        WRITE_REG(GPIOB->BSRR, ((GPIOB->ODR & GPIO_PIN_3) << 16u) | (~GPIOB->ODR & GPIO_PIN_3));       
    }    
}

void SystemDevice::delay_ms(uint32_t ms) {
    uint32_t start = g_systick;
    while (g_systick - start < ms);
}    



