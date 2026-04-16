#include <stdint.h>
#include "STM32F405xx.h"

class GPIO
{
public:
    static void enable_clock()
    {
        RCC->AHB1ENR |= (1U << 3);
    }

    static void set_output(uint32_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));
        GPIOD->MODER |=  (1U << (pin * 2));
    }

    static void set(uint32_t pin)
    {
        GPIOD->BSRR = (1U << pin);
    }

    static void reset(uint32_t pin)
    {
        GPIOD->BSRR = (1U << (pin + 16));
    }

    static void toggle(uint32_t pin)
    {
        if (GPIOD->ODR & (1U << pin))
            reset(pin);
        else
            set(pin);
    }
};

template<uint8_t pin>
class LED
{
private:
    LED()
    {
      GPIO::set_output(pin); //setting pin in output mode for led
    }

    ~LED() {

    	RCC->AHB1ENR &= ~(1U << 3);
    	  GPIOD->MODER &= ~(3U << (pin * 2));

    	  GPIOD->BSRR = (1U << (pin+16));
    }

public:
    static LED& instance()
    {
        static LED obj;
        return obj;
    }

    void led_on()
    {
        GPIO::set(pin);
    }

    void led_off()
    {
        GPIO::reset(pin);
    }

    void led_toggle()
    {
        GPIO::toggle(pin);
    }

    LED(const LED&) = delete;
    LED& operator=(const LED&) = delete;
};

void delay()
{
    for(volatile int i = 0; i < 50000; i++);
}

int main()
{
    GPIO::enable_clock();

    auto &  green = LED<12>::instance();
    auto & red   = LED<14>::instance();

    while(1)
    {
        green.led_on();
        delay();

        red.led_on();
        delay();

        green.led_off();
        delay();

        red.led_off();
        delay();
    }
}
