#include <stdint.h>

typedef struct
{
    volatile uint32_t MODER; //4 byte 0x00
    volatile uint32_t OTYPER;// 0x04
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} GPIO_RegDef_t;

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
} RCC_RegDef_t;

#define RCC_BASE   0x40023800
#define GPIOD_BASE 0x40020C00


#define RCC   ((RCC_RegDef_t*) RCC_BASE) // type casting
#define GPIOD ((GPIO_RegDef_t*) GPIOD_BASE)


class LED
{
private:
    GPIO_RegDef_t *pGpio_port;
    uint8_t pin;

public:


    LED(GPIO_RegDef_t *port, uint8_t pin_num)
    {
        pGpio_port = port;
        pin = pin_num;
        RCC->AHB1ENR |= (1 << 3);

        pGpio_port->MODER &= ~(3 << (2 * pin));


        pGpio_port->MODER |= (1 << (2 * pin));
    }


    void toggle()
    {
        pGpio_port->ODR ^= (1 << pin);
    }
};
