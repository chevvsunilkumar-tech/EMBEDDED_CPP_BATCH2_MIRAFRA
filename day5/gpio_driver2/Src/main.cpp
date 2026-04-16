#include "STM32F405xx.h"
#include <stdint.h>

enum {A=12,B,C,D};

class GPIOClockGuard
{
public:
    GPIOClockGuard()
    {
        RCC->AHB1ENR |= (1U << 3); // Enable GPIOD
        RCC->AHB1ENR |= (1U << 0); // Enable GPIOA
    }

    ~GPIOClockGuard()
    {
        RCC->AHB1ENR &= ~(1U << 3); // Disable GPIOD
        RCC->AHB1ENR &= ~(1U << 0); // Disable GPIOA
    }
};

class GPIO
{
public:

    static void set_output(uint32_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));
        GPIOD->MODER |=  (1U << (pin * 2)); // Output mode
    }

    static void set_input(uint32_t pin)
    {
        GPIOA->MODER &= ~(3U << (pin * 2)); // Input mode
    }

    static void toggle(uint32_t pin)
    {
        GPIOD->ODR ^= (1U << pin);
    }

    static bool read(uint32_t pin)
    {
        return (GPIOA->IDR & (1U << pin));
    }
};


void delay()
{

    for (volatile uint32_t i = 0; i < 100000; ++i)
    {
    }
}

int main()
{
    {
        GPIOClockGuard clock;

        GPIO::set_input(0);

        GPIO::set_output(A);
        GPIO::set_output(B);
        GPIO::set_output(C);
        GPIO::set_output(D);

        while (1)
        {
            GPIO::toggle(A);
            GPIO::toggle(B);
            GPIO::toggle(C);
            GPIO::toggle(D);

            delay();

            if (GPIO::read(0))
            {
                break;
            }
        }
    }


}
