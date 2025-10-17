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


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*
*  Built in LED3 -> PC6 (not connected to headers)
*/

void Delay(int milliseconds);//Function prototype (Local)

int main(void) 
{
  /*Initialization*/

  //Enable power interface clock (RM 5.4.15)
  RCC->APBENR1 |= RCC_APBENR1_PWREN;

  /*Peripherals clock enable (RM 5.4.13)*/
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN; //connects GPIOB to the clock

  /*set PC6 as ouput*/
  //GPIOC->MODER &= ~GPIO_MODER_MODE6_Msk; // Clear mode bits
  GPIOC->MODER &= ~(0b11<<(2*6)); // Clear mode bits
  //GPIOC->MODER |= GPIO_MODER_MODE6_0;    // Set to output (01)
  GPIOC->MODER |= 0b01<<(2*6);

  printf("Hello World");

  /*Infinite loop*/
  while (1)
  {
    //GPIOC->ODR ^= GPIO_ODR_OD6;
    GPIOC->ODR ^= 0b1<<6;
    Delay(500);
  }

}

//Function definition
void Delay(int milliseconds)
{
  for (int i = 0 ; i < milliseconds ; ++i)
    for (int j = 0 ; j < 571 ; ++j)
      asm("nop"); //takes 1 cycle;
}

/*************************** End of file ****************************/

/* Challenge

- Try to create a function that delays for 1[ms]
- Try to create another function that delays for X [ms] where x is passed in
as a parameter.

*/
