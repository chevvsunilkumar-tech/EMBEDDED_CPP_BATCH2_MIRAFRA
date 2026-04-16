#include <stdint.h>
#include <stdio.h>
#include "STM32F405xx.h"


extern "C" void UsageFault_Handler(void);
extern "C" void HardFault_Handler(void);
extern "C" void MemManage_Handler(void);
extern "C" void BusFault_Handler(void);
extern "C" void UsageFault_Handler_c(uint32_t *pMSP);


void USART3_init(void);
extern "C" void USART3_write(uint8_t ch);
uint8_t USART3_read(void);


int fun_divide(int x , int y)
{
    return x / y;
}


int main(void)
{
    USART3_init();


    uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

    *pSHCSR |= (1 << 16); // MemManage
    *pSHCSR |= (1 << 17); // BusFault
    *pSHCSR |= (1 << 18); // UsageFault ENABLED

    // Enable divide-by-zero trap
    uint32_t *pCCR = (uint32_t*)0xE000ED14;
    *pCCR |= (1 << 4);// enable DIV_0 TRAP

    volatile int a = 10;
    volatile int b = 0;

    fun_divide(a, b);   // UsageFault

    while(1);
}


void USART3_init(void)
{
    RCC->AHB1ENR |= (1 << 2);   // GPIOC clock
    RCC->APB1ENR |= (1 << 18);  // USART3 clock

    // PC10 → TX
    GPIOC->MODER &= ~(3 << 20);
    GPIOC->MODER |=  (1 << 21);
    GPIOC->AFR[1] &= ~(0xF << 8);
    GPIOC->AFR[1] |=  (7 << 8);

    // PC11 → RX
    GPIOC->MODER &= ~(3 << 22);
    GPIOC->MODER |=  (1 << 23);
    GPIOC->AFR[1] &= ~(0xF << 12);
    GPIOC->AFR[1] |=  (7 << 12);

    USART3->BRR = 0x0683;      // 9600 baud
    USART3->CR1 |= (1 << 3);   // TE
    USART3->CR1 |= (1 << 2);   // RE
    USART3->CR1 |= (1 << 13);  // UE
}


extern "C" void USART3_write(uint8_t ch)
{
    while (!(USART3->SR & (1 << 7)));
    USART3->DR = ch;
    while (!(USART3->SR & (1 << 6)));
}




uint8_t USART3_read(void)
{
    while (!(USART3->SR & (1 << 5)));
    return USART3->DR;
}



extern "C" void HardFault_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}

extern "C" void MemManage_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}

extern "C" void BusFault_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}


extern "C" __attribute__((naked)) void UsageFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP        \n"
        "B UsageFault_Handler_c \n"
    );
}


extern "C" void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pUFSR = (uint32_t*)0xE000ED2A;

    printf("\n--- UsageFault Occurred ---\n");
    printf("UFSR = %lx\n", (*pUFSR) & 0xFFFF);

    uint8_t status= (*pUFSR & (1<<9));
    printf("statux  = %lx\n",status);

    printf("R0  = %lx\n", pBaseStackFrame[0]);
    printf("R1  = %lx\n", pBaseStackFrame[1]);
    printf("R2  = %lx\n", pBaseStackFrame[2]);
    printf("R3  = %lx\n", pBaseStackFrame[3]);
    printf("R12 = %lx\n", pBaseStackFrame[4]);
    printf("LR  = %lx\n", pBaseStackFrame[5]);
    printf("PC  = %lx\n", pBaseStackFrame[6]);
    printf("XPSR= %lx\n", pBaseStackFrame[7]);

    __asm("BKPT #0");

    while(1);
}
