#include "STM32F405xx.h"
#include <stdint.h>

// USART3_TX → PC10
// USART3_RX → PC11

uint8_t read_Ch;


void USART3_init(void);
void USART3_write(uint8_t ch);
uint8_t USART3_read(void);
void USART3_string(char *ch);
void delayMs(int);

int main(void)
{
    USART3_init();   // Initialize UART

   while (1)
    {
       USART3_string("sunil");
       delayMs(500);


   }
}

void USART3_init(void)
{
    RCC->AHB1ENR |= (1 << 2);   // Enable GPIOC clock
    RCC->APB1ENR |= (1 << 18);  // Enable USART3 clock

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

    //  Added pull-up (important)
    GPIOC->PUPDR &= ~(3 << 22);
    GPIOC->PUPDR |=  (1 << 22);

    // USART settings
    USART3->BRR = 0x0683;

    USART3->CR1 |= (1 << 3);   // TE
    USART3->CR1 |= (1 << 2);   //RE enabled
    USART3->CR1 |= (1 << 13);  // UE
}

/*----------------------------------------------------------*/
void USART3_write(uint8_t ch)
{
    while (!(USART3->SR & (1 << 7))); // TXE
    USART3->DR = ch;

    while (!(USART3->SR & (1 << 6))); // TC (kept as you wrote)
}

/*----------------------------------------------------------*/
uint8_t USART3_read(void)
{
    while (!(USART3->SR & (1 << 5))); // RXNE
    return USART3->DR;
}

/*----------------------------------------------------------*/
void USART3_string(char *ch)
{
    for(int i=0; ch[i]; i++)
    {
        while (!(USART3->SR & (1 << 7)));
        USART3->DR = ch[i];
        while (!(USART3->SR & (1 << 6)));
        read_Ch= USART3_read();

    }
}

/*----------------------------------------------------------*/
void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
