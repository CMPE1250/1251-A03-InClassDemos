/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include "stm32g031xx.h"
#include "gpio.h"


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*
*  Built in LED3 -> PC6 (not connected to headers)
*/

void Delay(void);//Function prototype (Local)

int main(void) 
{
  /*Initialization*/

  //Enable power interface clock (RM 5.4.15)
  RCC->APBENR1 |= RCC_APBENR1_PWREN;

  /*Peripherals clock enable (RM 5.4.13)*/
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN; //connects GPIOB to the clock

  /*set PB3 as ouput*/
  //GPIOB->MODER &= ~(0b11<<(2*3)); // Clear mode bits
  //GPIOB->MODER |= 0b01<<(2*3);
  _GPIO_SetPinMode(GPIOB, 3, _GPIO_PinMode_Output);



  printf("Hello World");


  /*Infinite loop*/
  while (1)
  {
    GPIOB->ODR ^= 0b1<<3;
    Delay();
  }

}

//Function definition
void Delay(void)
{
  asm("nop"); //takes 1 cycle;
}

/*************************** End of file ****************************/

