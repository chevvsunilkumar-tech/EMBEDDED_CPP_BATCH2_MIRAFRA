#include "sw_led.h"


void SW_LED::on()
{
    l_pGPIOx->ODR |= (1 << l_pin);
}

void SW_LED::off()
{
    l_pGPIOx->ODR &= ~(1 << l_pin);
}

void SW_LED::toggle()
{
    l_pGPIOx->ODR ^= (1 << l_pin);
}

bool SW_LED::isswitch_on()
{
    return (sw_pGPIOx->IDR & (1 << sw_pin));
}
