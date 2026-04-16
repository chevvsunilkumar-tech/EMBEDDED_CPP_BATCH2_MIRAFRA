int main()
{
    unsigned int *ahb1 = (unsigned int*)0x40023830;
    unsigned int *gpio_odr = (unsigned int*)0x40020C14;
    unsigned int *gpiod_moder = (unsigned int*)0x40020C00;


    *ahb1 |= (1 << 3);


    *gpiod_moder &= ~(3 << 24);
    *gpiod_moder |= (1 << 24);

    while(1)
    {
        *gpio_odr |= (1 << 12); //to turn on led

        for( int i = 0; i < 30000; i++);

        *gpio_odr &= ~(1 << 12); // to turn off led

        for(int i = 0; i < 30000; i++);
    }
}
