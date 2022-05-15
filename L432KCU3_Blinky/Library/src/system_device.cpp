#include <system_device.hpp>

/// @brief The system clock tick
volatile uint32_t g_systick {0};

namespace stm32 {

// set system clock frequency to 80MHz
SystemDevice::SystemDevice()
{
    uint32_t nvic_prioritygroup_4 = static_cast<uint32_t>(0x00000003);
    NVIC_SetPriorityGrouping(nvic_prioritygroup_4);

    [[maybe_unused]]  __IO uint32_t tmpreg; 

    // Enable the PWR clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    tmpreg = RCC->APB1ENR1 & RCC_APB1ENR1_PWREN;

    // Enable the FLASH clock
    RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;
    tmpreg = RCC->AHB1ENR & RCC_AHB1ENR_FLASHEN;

    // Set the VCore to "High-performance range" and wait until VOSF flag is cleared.
    PWR->CR1 |= PWR_CR1_VOS_0;
    while((PWR->SR2 & PWR_SR2_VOSF));

    // Set flash latency "wait states" required for HCLK @ 80MHz
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);
    FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
    // Enable flash prefetch
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    // Enable the High-speed internal osc and wait until it is ready
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    // Set the PLL source to HSI16 (16MHz)
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
    // Set the PLLM prescaler to /1 (16MHz)
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
    // Set the PLLN multiplier to x10 (160MHz) 
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLN_1 | RCC_PLLCFGR_PLLN_3);
    // Set the PLLR prescaler to /2 (80MHz)
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;
    // Enable the PLLR output
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    // Select the PLL as the system clock
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= (RCC_CFGR_SW_0 | RCC_CFGR_SW_1);
    while(!(RCC->CFGR & (RCC_CFGR_SW_0 | RCC_CFGR_SW_1)));

    // Set the APBx, AHB1, AHB2 prescalers to /1 (80MHz)
    RCC->CFGR &= ~RCC_CFGR_HPRE;
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR &= ~RCC_CFGR_PPRE2;

    // Finally, enable the main PLL and wait until it is locked.
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / (1000U));
   
}

void SystemDevice::delay_ms(uint32_t ms) {
    uint32_t start = g_systick;
    while (g_systick - start < ms);
}    


} // namespace stm32
