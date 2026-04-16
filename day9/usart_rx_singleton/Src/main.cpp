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

//usart1
class USART1_Driver
{
private:
    USART1_Driver()
    {
        RCC->AHB1ENR |= (1 << 1);   // GPIOB
        RCC->APB2ENR |= (1 << 4);   // USART1

        // PB6 → TX
        GPIOB->MODER &= ~(3 << 12);
        GPIOB->MODER |=  (1 << 13);
        GPIOB->AFR[0] &= ~(0xF << 24);
        GPIOB->AFR[0] |=  (7 << 24);

        // PB7 → RX
        GPIOB->MODER &= ~(3 << 14);
        GPIOB->MODER |=  (1 << 15);
        GPIOB->AFR[0] &= ~(0xF << 28);
        GPIOB->AFR[0] |=  (7 << 28);

        USART1->BRR = 0x0683;
        USART1->CR1 |= (1 << 2);   // RE
        USART1->CR1 |= (1 << 13);  // UE
    }

public:
    static USART1_Driver& instance()
    {
        static USART1_Driver obj;
        return obj;
    }

    uint8_t read()
    {
        while (!(USART1->SR & (1 << 5))); // RXNE
        return USART1->DR;
    }
};

// LED
class LEDController
{
private:
    uint8_t read_Ch = 0;


    LEDController()
    {
        RCC->AHB1ENR |= (1 << 1); // GPIOB
        RCC->AHB1ENR |= (1 << 4); // GPIOE

        // PB4
        GPIOB->MODER &= ~(3 << (4 * 2));
        GPIOB->MODER |=  (1 << (4 * 2));

        // PB3
        GPIOB->MODER &= ~(3 << (3 * 2));
        GPIOB->MODER |=  (1 << (3 * 2));

        // PE2
        GPIOE->MODER &= ~(3 << (2 * 2));
        GPIOE->MODER |=  (1 << (2 * 2));

        // PE5
        GPIOE->MODER &= ~(3 << (5 * 2));
        GPIOE->MODER |=  (1 << (5 * 2));
    }

public:
    static LEDController& instance()
    {
        static LEDController obj;
        return obj;
    }

    void process(USART1_Driver& usart)
    {
        read_Ch = usart.read();

        if (read_Ch == '1')
        {

            GPIOB->BSRR = (1 << 4);
            Delay::ms(1000);
            GPIOB->BSRR = (1 << (4 + 16));
        }
        else if (read_Ch == '2')
        {
            GPIOB->BSRR = (1 << 3);
            Delay::ms(1000);
            GPIOB->BSRR = (1 << (3 + 16));
        }
        else if (read_Ch == '3')
        {
            GPIOE->BSRR = (1 << 2);
            Delay::ms(1000);
            GPIOE->BSRR = (1 << (2 + 16));
        }
        else if (read_Ch == '4')
        {
            GPIOE->BSRR = (1 << 5);
            Delay::ms(1000);
            GPIOE->BSRR = (1 << (5 + 16));
        }

        read_Ch = 0;
    }
};

//main
int main()
{
    auto& usart = USART1_Driver::instance();
    auto& leds  = LEDController::instance();

    while (1)
    {
        leds.process(usart);
    }
}
