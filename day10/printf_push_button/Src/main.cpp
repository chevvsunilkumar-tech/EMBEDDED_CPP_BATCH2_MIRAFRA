#include <stdio.h>
#include "STM32F405xx.h"

void delay(int t)
{
    for(int i = 0; i < t ; i++);
}

void init()
{
    RCC->AHB1ENR |= (1 << 0);
    RCC->AHB1ENR |= (1 << 3);
    GPIOA->MODER &= ~(3 << 0);

    GPIOD->MODER &= ~(3 << 24);
    GPIOD->MODER |= (1 << 24);
}

void print()
{

    printf("hello world\n");
    GPIOD->ODR|=1<<12;
    delay(3000000);
}

int main(void)
{
    int prev = 0;
    int cur  = 0;

    init();
    while (1)
    {
        cur = GPIOA->IDR & 1;

        if (prev == 0 && cur == 1)
        {
            delay(20);

            if (GPIOA->IDR & 1)
            {
                print();
            }
        }

        prev = cur;
    }
}
