#include "STM32F405xx.h"
#include <stdint.h>

// USART3_TX → PC10
// USART3_RX → PC11

uint8_t read_Ch;
class Usart3
{

static inline  void USART3_init(void);
static inline  void USART3_write(uint8_t ch);
static inline  uint8_t USART3_read(void);

friend int main(void);

};

void delayMs(int);

int main(void)
{
	Usart3::USART3_init();   // Initialize UART

   while (1)
    {
	   Usart3::  USART3_write('A');// 1 byte
        delayMs(500);

   }
}

 inline  void Usart3::USART3_init(void)
{
    RCC->AHB1ENR |= (1 << 2);   // Enable GPIOC clock
    RCC->APB1ENR |= (1 << 18);  // Enable USART3 clock

    // PC10 → TX
    GPIOC->MODER &= ~(3 << 20); // pin10 =pin*2, claered the bits
    GPIOC->MODER |=  (1 << 21);   // Alternate function -10
    // created Array :AFR[2]  , AFR[0]=AFRL ,AFR[1]= AFRH
    GPIOC->AFR[1] &= ~(0xF << 8); // AFRH =AFR[1]- Clearing 4 bits
    GPIOC->AFR[1] |=  (7 << 8);   // AF7 (USART3) // 0111 for AF7

    // PC11 → RX
    GPIOC->MODER &= ~(3 << 22);
    GPIOC->MODER |=  (1 << 23);// 10 AFR mode
    GPIOC->AFR[1] &= ~(0xF << 12);// AFRH Register
    GPIOC->AFR[1] |=  (7 << 12); // AF7 -0111

    // USART settings
    USART3->BRR = 0x0683;      // 9600 baud @ 16 MHz(HSI)
    USART3->CR1 |= (1 << 3);   // TE
    //USART3->CR1 |= (1 << 2);   // RE
    USART3->CR1 |= (1 << 13);  // UE - USART enable
}

/*----------------------------------------------------------
  Write one byte
----------------------------------------------------------*/
inline  void Usart3:: USART3_write(uint8_t ch) //ch ='A'
{
    // Wait until DR empty
    while (!(USART3->SR & (1 << 7))); // TXE =0


    USART3->DR = ch;

    // Wait until transmission complete
    while (!(USART3->SR & (1 << 6))); // TC=0
}
 inline  uint8_t Usart3::USART3_read(void)
{
    while (!(USART3->SR & (1 << 5))); // RXNE
    return USART3->DR;
}

/*----------------------------------------------------------
  Delay
----------------------------------------------------------*/
void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
