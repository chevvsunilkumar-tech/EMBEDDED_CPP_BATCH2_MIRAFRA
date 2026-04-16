#include "STM32F405xx.h"
#include <stdint.h>

uint8_t read_Ch='A';

uint8_t status=0;

void USART1_init(void);
void USART3_init(void);
void USART1_write(uint8_t ch);
uint8_t USART3_read(void);
void LED_init(void);
void delayMs(int);


int main(void)
{
    USART1_init();   // Sender
    USART3_init();   // Receiver
    LED_init();

    while (1)
    {
        USART1_write(read_Ch);   // Send character from USART1
        delayMs(500);

        read_Ch = USART3_read();   // Receive at USART3

        if (read_Ch == 'Z')
        {
            // Blink LED
            GPIOD->BSRR = (1 << 12);       // LED ON
            delayMs(200);

            GPIOD->BSRR = (1 << (12 + 16)); // LED OFF
            delayMs(200);
        }
        if(read_Ch=='Z')
        	read_Ch='A';
        else
        	read_Ch++;
    }
}


/* USART1
 *  PB6 tx, PB7 rx */
void USART1_init(void)
{
    RCC->AHB1ENR |= (1 << 1);   // GPIOB clock
    RCC->APB2ENR |= (1 << 4);   // USART1 clock

    // PB6 → TX
    GPIOB->MODER &= ~(3 << 12);
    GPIOB->MODER |=  (2 << 12);
    GPIOB->AFR[0] |= (7 << 24);

    // PB7 → RX
    GPIOB->MODER &= ~(3 << 14);
    GPIOB->MODER |=  (2 << 14);
    GPIOB->AFR[0] |= (7 << 28);

    USART1->BRR = 0x0683;  // 9600 baud @16MHz
    USART1->CR1 |= (1 << 3); // TE
    USART1->CR1 |= (1 << 2); // RE
    USART1->CR1 |= (1 << 13); // UE
}


/* USART3 INIT → PC10 (TX), PC11 (RX) */
void USART3_init(void)
{
    RCC->AHB1ENR |= (1 << 2);   // GPIOC
    RCC->APB1ENR |= (1 << 18);  // USART3

    // PC10 → TX
    GPIOC->MODER &= ~(3 << 20);
    GPIOC->MODER |=  (2 << 20);
    GPIOC->AFR[1] |= (7 << 8);

    // PC11 → RX
    GPIOC->MODER &= ~(3 << 22);
    GPIOC->MODER |=  (2 << 22);
    GPIOC->AFR[1] |= (7 << 12);

    // Pull-up for RX
    GPIOC->PUPDR &= ~(3 << 22);
    GPIOC->PUPDR |=  (1 << 22);

    USART3->BRR = 0x0683;

    USART3->CR1 |= (1 << 3); // TE
    USART3->CR1 |= (1 << 2); // RE
    USART3->CR1 |= (1 << 13); // UE
}

/* Send from USART1 */
void USART1_write(uint8_t ch)
{
    while (!(USART1->SR & (1 << 7))); // TXE
    USART1->DR = ch;
}


/* Receive from USART3 */
uint8_t USART3_read(void)
{
    while (!(USART3->SR & (1 << 5))); // RXNE
    return USART3->DR;
}


/* LED on PD12 */
void LED_init(void)
{
    RCC->AHB1ENR |= (1 << 3);

    GPIOD->MODER &= ~(3 << (12 * 2));
    GPIOD->MODER |=  (1 << (12 * 2));
}


void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
