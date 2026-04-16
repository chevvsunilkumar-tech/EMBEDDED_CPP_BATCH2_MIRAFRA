#pragma once

#include "STM32F405xx.h"
#include <cstdint>

class SW_LED
{
private:
    volatile GPIO_TypeDef* l_pGPIOx;
    volatile GPIO_TypeDef* sw_pGPIOx;
    uint8_t l_pin;
    uint8_t sw_pin;

public:
  SW_LED(volatile GPIO_TypeDef* l_GPIOx, uint8_t l_pinNumber,
                   volatile GPIO_TypeDef* sw_GPIOx, uint8_t sw_pinNumber)
    {
        l_pGPIOx = l_GPIOx;
        l_pin = l_pinNumber;

        // Enable clock for LED GPIO
        if (l_pGPIOx == GPIOD)
            RCC->AHB1ENR |= (1 << 3);

        // Configure LED pin as output
        l_pGPIOx->MODER &= ~(3 << (l_pin * 2));
        l_pGPIOx->MODER |=  (1 << (l_pin * 2));

        sw_pGPIOx = sw_GPIOx;
        sw_pin = sw_pinNumber;

        // Enable clock for switch GPIO
        if (sw_pGPIOx == GPIOA)
            RCC->AHB1ENR |= (1 << 0);

        // Configure switch pin as input
        sw_pGPIOx->MODER &= ~(3 << (sw_pin * 2));
    }

    void on();
    void off();
    void toggle();
    bool isswitch_on();
};
