#include <stdint.h>
#include "STM32F405xx.h"
#include "sw_led.h"

int main(void)
{
    SW_LED led((GPIO_TypeDef*)GPIOD_BASE, 12,
               (GPIO_TypeDef*)GPIOA_BASE, 0);

    while (1)
    {
        if (led.isswitch_on())
            led.toggle();
    }
}
