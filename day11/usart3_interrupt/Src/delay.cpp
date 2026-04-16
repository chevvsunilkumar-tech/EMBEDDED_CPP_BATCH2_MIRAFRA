#include "STM32F405xx.h"
#include <stdint.h>


void TIM2_init(void)
{
    RCC->APB1ENR |= (1 << 0);   // Enable TIM2 clock

    TIM2->PSC = 1600 - 1;
    TIM2->ARR = 10000 - 1;
    TIM2->CNT = 0;
    TIM2->CR1 |= (1 << 0);      // Start timer
}

void delaySec(uint32_t sec)
{
    for(uint32_t i = 0; i < sec; i++)
    {
        TIM2->CNT = 0;

        while (!(TIM2->SR & (1 << 0))); // Wait for overflow

        TIM2->SR &= ~(1 << 0); // Clear UIF
    }
}
