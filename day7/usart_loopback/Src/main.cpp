//Jeena _UART
#include "STM32F405xx.h"
#include<string>
//#include "lcd.h"
//USART3_TX PC10[51]
//USART3_RX PC11[52]
uint8_t read_Ch;
uint8_t ch;
uint8_t d='F';
void USART3_init(void);
void USART3_write(uint8_t);
uint8_t USART3_read(void);
void delayMs(int);

void LED_init(void)
{
    RCC->AHB1ENR |= (1 << 3); // Enable GPIOD clock

    GPIOD->MODER &= ~(3 << (12 * 2));
    GPIOD->MODER |=  (1 << (12 * 2)); // Output mode
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

	//LcdInit();
	//lprint(0X80,"HELLO");
    USART3_init();          /* initialize USART3 */
    LED_init();

    while(1) {
    	/* Loop forever */

        USART3_write(d);
        delayMs(100);
       /* leave a gap between messages */
        read_Ch= USART3_read();//reading the character and storing in a variable
        d=d+1;

        if (read_Ch == 'Z')
               {
        	 if(GPIOD->ODR & (1U << 12))
            	   continue;
                   GPIOD->BSRR = (1 << 12);         // LED ON


            	   }
               else
               {
            	   if(read_Ch>'Z')
                 	   d='A';

            	   if(!(GPIOD->ODR & (1U << 12)))
            	              	   continue;

                   GPIOD->BSRR = (1 << (12 + 16));  // LED OFF

               }

    }
}

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void USART3_init (void) {
    RCC->AHB1ENR |= (1<<2);         // Enable GPIOC clock
    RCC->APB1ENR |= (1<<18);       //Enable USART3 clock
//PC10 is the TX PIN for USART3

   GPIOC->MODER &=~(3<<20); //PC10 cleared  bit 21,20
   GPIOC->MODER |=(1<<21); //PC10 in AFR mode, set bit 21
   GPIOC->AFR[1] &= ~(0XF<<8);// AFRH since pin10, Clearing the corresponding bits-Bit 11 10 9 8
   GPIOC->AFR[1] |= (7<<8);   //AF7(0111) -Setting bits 10 9 8

   //PC11 is the RX PIN for USART3

   GPIOC->MODER &=~(3<<22); //PC11, cleared  bit 23,22
   GPIOC->MODER |=(1<<23); //PC11 in AFR mode, set bit 23, AF=10
   GPIOC->AFR[1] &= ~(0XF<<12);// AFRH since pin11, Clearing the corresponding bits-Bit 15 14 13 12
   GPIOC->AFR[1] |= (7<<12);   //AF7(0111) -Setting bits 14 13 12

   //USART3 Settings
    USART3->BRR = 0x0683;       // 9600 baud @ 16 MHz
    USART3->CR1 |= (0XC<<0);      //Enable RE and TE bits -1100 -bit 3,2
    USART3->CR2 =0;     // 1 stop bit(00)- Bits 13 12 corresponding to number of stop bits
    USART3->CR3 = 0;       // Default settings
    USART3->CR1 |= (1<<13);      //USART3 Enable
}


/* Write a character to USART3 */
void USART3_write (uint8_t ch) {
	//lprint(0X80,"Sending Data");
	//LcdFxn(0,0x01);

	//Bit 7 -TXEThis bit is set by hardware when the content of the TDR register has been transferred into
	//the shift register. An interrupt is generated if the TXEIE bit =1 in the USART_CR1 register
    while (!(USART3->SR & (1<<7))) {}   // wait until Tx buffer empty
    USART3->DR = (ch & 0xFF);//User passed characters are writing to DR Register
   // aprint(USART3->DR);
}
/* Read a character from UART3 */
uint8_t USART3_read(void) {
	//lprint(0XC0,"Receiving Data");
	//SR->bit 5 RXNE interrupt enable,This bit is set and cleared by software.
	//0: Interrupt is inhibited
	//1: An USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register Bit 4 IDLEIE: IDLE int
    while (!(USART3->SR &(1<<5))) {}   // wait until char arrives,
    return USART3->DR;
}
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 2000; i++) ;
}
