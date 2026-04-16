#include <stdint.h>

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
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

constexpr uint32_t RCC_BASE   = 0x40023800;
constexpr uint32_t GPIOD_BASE = 0x40020C00;
constexpr uint32_t GPIOA_BASE = 0x40020000;
constexpr uint32_t GPIOA_IDR = 0x40020010;


inline volatile RCC_RegDef_t& RCC = *reinterpret_cast<volatile RCC_RegDef_t*>(RCC_BASE);

inline volatile GPIO_RegDef_t& GPIOD = *reinterpret_cast<volatile GPIO_RegDef_t*>(GPIOD_BASE);
inline volatile GPIO_RegDef_t& GPIOA= *reinterpret_cast<volatile GPIO_RegDef_t*>(GPIOA_BASE);

class SW_LED
{
private:
    volatile GPIO_RegDef_t* l_pGPIOx;
    volatile GPIO_RegDef_t* sw_pGPIOx;
    uint8_t l_pin;
    uint8_t sw_pin;

public:
    SW_LED(volatile GPIO_RegDef_t* l_GPIOx, uint8_t l_pinNumber,volatile GPIO_RegDef_t* sw_GPIOx, uint8_t sw_pinNumber)
    {
        l_pGPIOx = l_GPIOx;
        l_pin = l_pinNumber;
        // Enable clock for GPIOD
        if (l_pGPIOx == &GPIOD)
            RCC.AHB1ENR |= (1 << 3);

        // Configure mode
        l_pGPIOx->MODER &= ~(3 << (l_pin * 2));
        l_pGPIOx->MODER |=  (1 << (l_pin * 2));

        sw_pGPIOx = sw_GPIOx;
              sw_pin = sw_pinNumber;
              // Enable clock for GPIOD
              if (sw_pGPIOx == &GPIOA)
                  RCC.AHB1ENR |= (1 << 0);

              // Configure mode
              sw_pGPIOx->MODER &= ~(3 << (sw_pin * 2));
              sw_pGPIOx->MODER |=  (0 << (sw_pin * 2));
    }

    void on()
    {
        l_pGPIOx->ODR |= (1 << l_pin);
    }

    void off()
    {
        l_pGPIOx->ODR &= ~(1 << l_pin);
    }

    void toggle()
    {
        l_pGPIOx->ODR ^= (1 << l_pin);
    }

    bool isswitch_on()
    {
    	if (sw_pGPIOx->IDR & (1 << sw_pin))
    	    return true;
    	return false;

    }
};

int main(void)
{
    SW_LED led(&GPIOD, 12,&GPIOA,0);   // pass address

    while (1)
    {
      if(led.isswitch_on())
    	  led.toggle();


    }
}
