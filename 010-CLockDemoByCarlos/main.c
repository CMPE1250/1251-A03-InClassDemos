/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include "stm32g031xx.h"
#include "gpio.h"
#include <stdio.h>

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*
*/


int main(void) 
{
  /*Initialization*/
  //Enable power interface clock (RM 5.4.15)
  RCC->APBENR1 |= RCC_APBENR1_PWREN;

  /*Peripherals clock enable (RM 5.4.13)*/
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN; //connects GPIOA to the clock
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN; //connects GPIOB to the clock
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN; //connects GPIOC to the clock

  /*We want to check MCO (Master clock output) out on PA8*/
  _GPIO_SetPinMode(GPIOA, 8, _GPIO_PinMode_AlternateFunction);
  _GPIO_SetPinAlternateFunction(GPIOA, 8, 0);

  RCC->CFGR &= ~RCC_CFGR_MCOSEL;        // Clear MCO source
  RCC->CFGR |= 1 <<24;  // Select SYSCLK as MCO source
  //RCC->CFGR |= RCC_CFGR_MCOSEL_0; //this will also work as previous line
  RCC->CFGR &= ~RCC_CFGR_MCOPRE;  //clear mco prescale
  RCC->CFGR |= RCC_CFGR_MCOPRE_1;  //divide by 4

  printf("The system clock is", SystemCoreClock);

  /*Infinite loop*/
  while (1)
  {

  }
}


/*************************** End of file ****************************/

