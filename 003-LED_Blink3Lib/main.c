/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

//#include "stm32g031xx.h"
#include "gpio.h"
#include <stdio.h>

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
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN; //connects GPIOC to the clock

  /*set PB3 as ouput*/
  //GPIOC->MODER &= ~(0b11<<(2*3)); // Clear mode bits
  //GPIOC->MODER |= 0b01<<(2*3);
  _GPIO_SetPinMode(GPIOC, 6, _GPIO_PinMode_Output);



  printf("Hello World");


  /*Infinite loop*/
  while (1)
  {
    //GPIOB->ODR ^= 0b1<<3;
    _GPIO_PinToggle(GPIOC, 6);
    utiDelay(500); //Wait a half sec 
  }

}

//Function definition
void Delay(void)
{
  asm("nop"); //takes 1 cycle;
}

/*************************** End of file ****************************/

