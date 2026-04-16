#include <stdint.h>
#include "STM32F405xx.h"

class GPIO
{
public:
    static void enable_clock()
    {
        RCC->AHB1ENR |= (1U << 3); // Enable GPIOD clock
    }

    static void set_output(uint32_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));  // Clear mode
        GPIOD->MODER |=  (1U << (pin * 2));  // Set as output
    }

    static void set(uint32_t pin)
    {
        GPIOD->BSRR = (1U << pin); // Use '=' (not |=)
    }

    static void reset(uint32_t pin)
    {
        GPIOD->BSRR = (1U << (pin + 16)); // Reset bit
    }

    static void toggle(uint32_t pin)
    {
        if (GPIOD->ODR & (1U << pin))
            reset(pin);
        else
            set(pin);
    }
};


class LED
{
    uint32_t pin;

public:
    explicit LED(uint32_t p) : pin(p)
    {
        GPIO::set_output(pin);
    }

    void on()
    {
        GPIO::set(pin);
    }

    void off()
    {
        GPIO::reset(pin);
    }

    void toggle()
    {
        GPIO::toggle(pin);
    }
};


void delay()
{
    for (volatile uint32_t i = 0; i < 200000; ++i)
    {
    }
}


int main()
{
    constexpr uint32_t LED_PIN = 12;

    GPIO::enable_clock();

    LED led(LED_PIN);

    while (true)
    {
        led.toggle();
        delay();
    }
}
