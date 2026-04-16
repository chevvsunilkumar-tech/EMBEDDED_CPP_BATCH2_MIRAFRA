
#include"header.h"
int main()
{

LED led(GPIOD,12);
while(1)
{
led.toggle();

}


}
