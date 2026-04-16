#include <stdint.h>
#include "STM32F405xx.h"

//delay
class Delay
{
public:
    static void ms(int n)
    {
        for (; n > 0; n--)
            for (volatile int i = 0; i < 2000; i++);
    }
};

//usart3 for transmitter
class USART3_Driver
{
private:
    USART3_Driver()
    {
        RCC->AHB1ENR |= (1U << 2);   // GPIOC
        RCC->APB1ENR |= (1U << 18);  // USART3

        // PC10 TX
        GPIOC->MODER &= ~(3U << 20);
        GPIOC->MODER |=  (2U << 20);
        GPIOC->AFR[1] &= ~(0xFU << 8);
        GPIOC->AFR[1] |=  (7U << 8);

        // PC11 RX
        GPIOC->MODER &= ~(3U << 22);
        GPIOC->MODER |=  (2U << 22);
        GPIOC->AFR[1] &= ~(0xFU << 12);
        GPIOC->AFR[1] |=  (7U << 12);

        USART3->BRR = 0x0683;
        USART3->CR1 |= (1U << 3);  // TE
        USART3->CR1 |= (1U << 2);  // RE
        USART3->CR1 |= (1U << 13); // UE
    }

public:
    static USART3_Driver& instance()
    {
        static USART3_Driver obj;
        return obj;
    }

    void write(uint8_t ch)
    {
        while (!(USART3->SR & (1U << 7))); // TXE
        USART3->DR = ch;
        while (!(USART3->SR & (1U << 6))); // TC
    }
};

//push button
class ButtonController
{
private:
    uint8_t counter1 = 0;
    uint8_t counter2 = 0;
    uint8_t counter3 = 0;
    uint8_t counter4 = 0;

    uint8_t prev_status1 = 0;
    uint8_t prev_status2 = 0;
    uint8_t prev_status3 = 0;
    uint8_t prev_status4 = 0;

    ButtonController()
    {
        // Enable clocks
        RCC->AHB1ENR |= (1U << 0); // GPIOA
        RCC->AHB1ENR |= (1U << 1); // GPIOB
        RCC->AHB1ENR |= (1U << 2); // GPIOC
        RCC->AHB1ENR |= (1U << 3); // GPIOD

        // PA1
        GPIOA->MODER &= ~(3U << 2);
        GPIOA->PUPDR &= ~(3U << 2);
        GPIOA->PUPDR |=  (2U << 2);

        // PB1
        GPIOB->MODER &= ~(3U << 2);
        GPIOB->PUPDR &= ~(3U << 2);
        GPIOB->PUPDR |=  (2U << 2);

        // PC1
        GPIOC->MODER &= ~(3U << 2);
        GPIOC->PUPDR &= ~(3U << 2);
        GPIOC->PUPDR |=  (2U << 2);

        // PD1
        GPIOD->MODER &= ~(3U << 2);
        GPIOD->PUPDR &= ~(3U << 2);
        GPIOD->PUPDR |=  (2U << 2);
    }

public:
    static ButtonController& instance()
    {
        static ButtonController obj;
        return obj;
    }

    void process(USART3_Driver& usart)
    {
        uint8_t status1 = (GPIOA->IDR & (1U << 1)) ? 1 : 0;
        uint8_t status2 = (GPIOB->IDR & (1U << 1)) ? 1 : 0;
        uint8_t status3 = (GPIOC->IDR & (1U << 1)) ? 1 : 0;
        uint8_t status4 = (GPIOD->IDR & (1U << 1)) ? 1 : 0;

        // BUTTON 1
        if ((status1 == 1) && (prev_status1 == 0))
        {
            Delay::ms(50);
            if (GPIOA->IDR & (1U << 1))
            {
                counter1++;
                usart.write('1');
            }
        }

        // BUTTON 2
        if ((status2 == 1) && (prev_status2 == 0))
        {
            Delay::ms(50);
            if (GPIOB->IDR & (1U << 1))
            {
                counter2++;
                usart.write('2');
            }
        }

        // BUTTON 3
        if ((status3 == 1) && (prev_status3 == 0))
        {
            Delay::ms(50);
            if (GPIOC->IDR & (1U << 1))
            {
                counter3++;
                usart.write('3');
            }
        }

        // BUTTON 4
        if ((status4 == 1) && (prev_status4 == 0))
        {
            Delay::ms(50);
            if (GPIOD->IDR & (1U << 1))
            {
                counter4++;
                usart.write('4');
            }
        }

        // update previous
        prev_status1 = status1;
        prev_status2 = status2;
        prev_status3 = status3;
        prev_status4 = status4;
    }
};

//main
int main()
{
    auto& usart = USART3_Driver::instance();
    auto& buttons = ButtonController::instance();

    while (1)
    {
        buttons.process(usart);
    }
}
