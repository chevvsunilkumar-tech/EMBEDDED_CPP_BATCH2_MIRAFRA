#include <stdint.h>
#include "STM32F405xx.h"

void delay_ms(uint32_t time);
uint32_t c=0;


class LED
{
    int pin;

public:

    LED(int num) : pin(num)
    {
        GPIOD->MODER &= ~(3 << (num * 2));
        GPIOD->MODER |=  (1 << (num * 2));
    }

    ~LED()
    {
    }

    static void LED_clock()
    {
        RCC->AHB1ENR |= (1 << 3);
    }

    void on()
    {
        GPIOD->BSRR = (1 << pin);
    }

    void off()
    {
        GPIOD->BSRR = (1 << (pin + 16));
    }

    void toggle()
    {
        if (GPIOD->ODR & (1 << pin))
            off();
        else
            on();
    }
};

void switch_init()
{
    RCC->AHB1ENR |= (1 << 0);
    GPIOA->MODER &= ~(3 << 0);
}

void delay_ms(uint32_t time)
{
    while(time--)
    {
        for(volatile uint32_t i = 0; i < 5000; i++);
    }
}

void off_all(LED** leds, int size)
{
	c=1;
    for(int i = 0; i < size; i++)
    {
        leds[i]->on();   // turn OFF

    delay_ms(50);
    leds[i]->off();   // turn OFF
    }

    for(int i = 0; i < size; i++)
    {
        delete leds[i];   // free memory
    }
}
int main(void)
{
    LED::LED_clock();
    switch_init();

    LED* green  = new LED(12);

     LED* orange = new LED(13);
    LED* red    = new LED(14);

    LED* blue   = new LED(15);

    LED* leds[4] = {green, orange, red, blue};

    uint8_t prev_status = 0;
     uint8_t status = 0;

    while (1)
    {
        for(int i = 0; i < 4; i++)
            leds[i]->toggle();

        delay_ms(200);


        status = GPIOA->IDR & (1 << 0);

        if ((status == 1) && (prev_status == 0))
        {
            delay_ms(20);

            if (GPIOA->IDR & (1 << 0))
              {
            	try
            	{
            	    throw leds;
            	}
            	catch (LED** obj)
            	{
            	    off_all(obj, 4);
            	    break;
            	}
            	catch (...)
            	{
            	    off_all(leds, 4);
            	    break;
            	}
            }
        }

        prev_status =status;
    }

    while(1);
}
