#include "STM32F405xx.h"
#include <cstdint>

class GPIO
{
public:
    static void enable_clock() noexcept
    {
       RCC->AHB1ENR |= (1U << 3);
    }

    static void set_output(std::uint32_t pin) noexcept
    {

          GPIOD->MODER &= ~(3U << (pin * 2));


        GPIOD->MODER |=  (1U << (pin * 2));
    }

    static void toggle(std::uint32_t pin) noexcept
    {
        GPIOD->ODR ^= (1U << pin);
    }
};

void delay()
{
    for (volatile std::uint32_t i = 0; i < 100000; ++i);
}


int main()
{
    constexpr std::uint32_t LED_PIN = 14; // PD12

    GPIO::enable_clock();
    GPIO::set_output(LED_PIN);

    while (true)
    {
        GPIO::toggle(LED_PIN);
        delay();
    }
}
