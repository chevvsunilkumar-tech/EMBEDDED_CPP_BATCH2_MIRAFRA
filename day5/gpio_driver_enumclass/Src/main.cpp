#include "STM32F405xx.h"
#include <stdint.h>




enum class RCC_AHB1ENR_bits : uint32_t
{
    GPIOA_EN = 0,
   GPIOB_EN = 1,
    GPIOC_EN = 2,
    GPIOD_EN = 3
};

enum class GPIO_mode : uint32_t
{
	 INPUT  = 0b00,
    OUTPUT = 0b01,
	ALT    = 0b10,
	 ANALOG = 0b11


};

enum class LED : uint32_t
{
       GREEN = 12,
    ORANGE = 13,
    RED= 14,
    BLUE = 15
};


class GPIOClockGuard
{
public:
    GPIOClockGuard()
    {
        RCC->AHB1ENR |= (1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOD_EN));
        RCC->AHB1ENR |= (1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOA_EN));
    }

    ~GPIOClockGuard()
    {
        RCC->AHB1ENR &= ~(1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOD_EN));
        RCC->AHB1ENR &= ~(1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOA_EN));
    }
};


class GPIO
{
public:
    static void set_output(LED pin)//LED is enum class
    {
        uint32_t p = static_cast<uint32_t>(pin);

        GPIOD->MODER &= ~(3U << (p * 2));
        GPIOD->MODER |=  (static_cast<uint32_t>(GPIO_mode::OUTPUT) << (p * 2));
    }

    static void set_input(uint32_t pin)
    {
        GPIOA->MODER &= ~(3U << (pin * 2));
    }

    static void toggle(LED pin) // LED is enum class
    {
        uint32_t p = static_cast<uint32_t>(pin);
        GPIOD->ODR ^= (1U << p);
        if(GPIOD->ODR &(1<<p))
        	GPIOD->BSRR|=(1<<(16+p));
        else
        	GPIOD->BSRR|=1<<p;

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

        GPIO::set_output(LED::GREEN);
        GPIO::set_output(LED::ORANGE);
        GPIO::set_output(LED::RED);
        GPIO::set_output(LED::BLUE);

        while (1)
        {
            GPIO::toggle(LED::GREEN);
            GPIO::toggle(LED::ORANGE);
            GPIO::toggle(LED::RED);
            GPIO::toggle(LED::BLUE);

            delay();

            if (GPIO::read(0))
            {
                break;
            }
        }
    }


}
