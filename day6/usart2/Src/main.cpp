#include "STM32F405xx.h"
#include <stdint.h>



uint8_t read_Ch;

void USART1_init(void);
void USART1_write(uint8_t ch);
uint8_t USART1_read(void);
void delayMs(int);

int main(void)
{
    USART1_init();   // Initialize UART

   while (1)
    {
        USART1_write('A');// 1 byte
        delayMs(500);

   }
}

void USART1_init(void)
{
    RCC->AHB1ENR |= (1 << 1);   // Enable GPIOB clock
    RCC->APB2ENR |= (1 << 4);  // Enable USART1 clock

    // Pb6 → TX
    GPIOB->MODER &= ~(3 << 12); // pin10 =pin*2, claered the bits
    GPIOB->MODER |=  (1 << 13);   // Alternate function -10
    // created Array :AFR[2]  , AFR[0]=AFRL ,AFR[1]= AFRH
    GPIOB->AFR[0] &= ~(0xF << 24); // AFRH =AFR[1]- Clearing 4 bits
    GPIOB->AFR[0] |=  (7 << 24);   // AF7 (USART3) // 0111 for AF7

    // Pb7 → RX
    GPIOB->MODER &= ~(3 << 14);
    GPIOB->MODER |=  (1 << 15);// 10 AFR mode
    GPIOB->AFR[0] &= ~(0xF << 28);// AFRH Register
    GPIOB->AFR[0] |=  (7 << 28); // AF7 -0111

    // USART settings
    USART1->BRR = 0x0683;      // 9600 baud @ 16 MHz(HSI)
    USART1->CR1 |= (1 << 3);   // TE
    USART1->CR1 |= (1 << 2);   // RE
    USART1->CR1 |= (1 << 13);  // UE - USART enable
}

/*----------------------------------------------------------
  Write one byte
----------------------------------------------------------*/
void USART1_write(uint8_t ch) //ch ='A'
{
    // Wait until DR empty
    while (!(USART1->SR & (1 << 7))); // TXE =0 while(!0) - while loop(true)
    //TXE=1 Data in DR register will be given to shift register

    USART1->DR = ch; // new data will be loaded to DR

    // Wait until transmission complete
    while (!(USART1->SR & (1 << 6))); // TC=0 while(!0) while(T) waiting
}
uint8_t USART1_read(void)
{
    while (!(USART1->SR & (1 << 5))); // RXNE
    return USART1->DR;
}

/*----------------------------------------------------------
  Delay
----------------------------------------------------------*/
void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
