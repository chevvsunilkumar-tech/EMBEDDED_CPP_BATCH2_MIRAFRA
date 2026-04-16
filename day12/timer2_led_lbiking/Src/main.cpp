#include <stdint.h>
#include"STM32F405xx.h"

int main(void)
{
    /* RCC Registers */



    /* TIM2 Registers */
   /* uint32_t *pTIM2_CR1 = (uint32_t*)0x40000000;
    uint32_t *pTIM2_SR  = (uint32_t*)0x40000010;
    uint32_t *pTIM2_CNT = (uint32_t*)0x40000024;
    uint32_t *pTIM2_PSC = (uint32_t*)0x40000028;
    uint32_t *pTIM2_ARR = (uint32_t*)0x4000002C;
    */

    /* 1. Enable clocks */
    RCC->AHB1ENR |= (1 << 3);  // GPIOC enable
    RCC->APB1ENR |= (1 << 0);  // TIM2 enable

     GPIOD->MODER&=~(3<<(2*12));
     GPIOD->MODER|=(1<<(2*12));

    /* 3. Timer configuration for 1 second delay */

    // System clock = 16 MHz
    // Step 1: Prescaler → 16 MHz / 1600 = 10 kHz
    TIM2->PSC = 1600 - 1;

    // Step 2: ARR → 10,000 ticks → 1 second
    TIM2->ARR = 10000 - 1;

    /* 4. Reset counter */
    TIM2->CNT = 0;

    /* 5. Enable timer */
    TIM2->CR1 |= (1 << 0);

    while(1)
    {
        /* Wait until update event (UIF = 1) */
        while(!(TIM2->SR & (1 << 0)));

        /* Clear UIF flag */
        TIM2->SR &= ~(1 << 0);

        /* Toggle LED (PC6) */
        if(GPIOD->ODR&(1<<12))
        {
        	GPIOD->BSRR=1<<(12+16);
        }
        else
        {
        	GPIOD->BSRR=1<<(12);
        }
    }
}
