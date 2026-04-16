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


class LED
{
	uint8_t pin;
public:
    LED(int pin_num)
    {
    	pin=pin_num;

      GPIO::set_output(pin); //setting pin in output mode for led
    }

    ~LED() {

    	 // RCC->AHB1ENR &= ~(1U << 3);
    	  GPIOD->MODER &= ~(3U << (pin * 2));

    	  GPIOD->BSRR = (1U << (pin+16));
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


};

void delay()
{
    for(volatile int i = 0; i < 50000; i++);
}

int main()
{
    GPIO::enable_clock();

   auto *green =new LED(12);

   LED led_red(14);
   auto *red =&led_red;
   LED orange(13);


   for (volatile int i=0;i<20;i++)
   {
        green->led_on();
        delay();
        green->led_off();


        red->led_on();
        delay();
        red->led_off();


        orange.led_on();
        delay();
        orange.led_off();

        green->led_on();
        red->led_on();
        orange.led_on();

        delay();
        delay();

        green->led_off();
        red->led_off();
        orange.led_off();

    }


   delete green;
}
