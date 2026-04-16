#include <stdint.h>
#include<stdio.h>
#include "STM32F405xx.h"

static int count=1;

//delay
void delay_ms(int ms)
{
    for(int i = 0; i < ms * 4000; i++)
    {

    }
}

/* ---------------- OBSERVER ---------------- */
class ButtonObserver
{
public:
    virtual void onButtonPressed() = 0;
};

/* ---------------- SUBJECT (BUTTON) ---------------- */
class Button
{
    GPIO_TypeDef* port;
    uint16_t pin;

    ButtonObserver* observers[10];
    int count;

public:
    Button(GPIO_TypeDef* p, uint16_t pin)
        : port(p), pin(pin), count(0) {}

    void attach(ButtonObserver* obs)
    {
        if(count < 10)
            observers[count++] = obs;
    }

    int read()
    {
        return (port->IDR & (1 << pin)) ? 1 : 0;
    }

    void poll()
    {
        static int last_state = 0;
        int current = read();


        if(current == 1 && last_state == 0)
        {
            delay_ms(20); // debounce

            if(read())
            {count++;
                notify();
            }
        }

        last_state = current;
    }

private:
    void notify()
    {
        for(int i = 0; i < count; i++)
        {
            observers[i]->onButtonPressed();
        }
    }
};

//green led
class Green_Led : public ButtonObserver
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    Green_Led(GPIO_TypeDef* p, uint16_t pin = 12)
        : port(p), pin(pin) {}

    void onButtonPressed() override
    {
    	 printf("green observer got notified count= %lx\n",count);

        if(port->ODR & (1 << pin))
            port->BSRR = (1 << pin) << 16;
        else
            port->BSRR = (1 << pin);
    }
};

//ornage led
class Orange_Led : public ButtonObserver
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    Orange_Led(GPIO_TypeDef* p, uint16_t pin = 13)
        : port(p), pin(pin) {}

    void onButtonPressed() override
    {
           printf("orange observer got notified count= %lx",count);

        port->BSRR=1<<pin;
        delay_ms(50);

        port->BSRR = (1 << pin) << 16;


    }
};

//red led
class Red_Led : public ButtonObserver
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    Red_Led(GPIO_TypeDef* p, uint16_t pin = 14)
        : port(p), pin(pin) {}

    void onButtonPressed() override
    {
    	 printf("red observer got notified count= %lx",count);
    	port->BSRR = (1 << pin);
    	delay_ms(50);
            port->BSRR = (1 << pin) << 16;

        	delay_ms(50);
            port->BSRR = (1 << pin);
        	delay_ms(50);
             port->BSRR = (1 << pin) << 16;
         	delay_ms(50);


    }
};

//blue led
class Blue_Led : public ButtonObserver
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    Blue_Led(GPIO_TypeDef* p, uint16_t pin = 15)
        : port(p), pin(pin) {}

    void onButtonPressed() override
    {
    	 printf("red observer got notified count= %lx",count);
            port->BSRR = (1 << pin);
    }
};

/* ---------------- GPIO INIT ---------------- */
void GPIO_Init()
{
    // Enable clocks
    RCC->AHB1ENR |= (1 << 0); // GPIOA
    RCC->AHB1ENR |= (1 << 3); // GPIOD

    // PA0 → input
    GPIOA->MODER &= ~(3 << (0 * 2));


    for(int i = 12; i <= 15; i++)
    {
        GPIOD->MODER &= ~(3 << (i * 2));
        GPIOD->MODER |=  (1 << (i * 2));
    }
}

/* ---------------- MAIN ---------------- */
int main()
{
    GPIO_Init();

    // Subject
    Button button(GPIOA, 0);

    // Observers
    Green_Led green(GPIOD, 12);
    Orange_Led orange(GPIOD, 13);
    Red_Led red(GPIOD, 14);
    Blue_Led blue(GPIOD, 15);

    // Attach
    button.attach(&green);
    button.attach(&orange);
    button.attach(&red);
    button.attach(&blue);

    while(1)
    {
        button.poll();
        delay_ms(10);
    }
}
