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
#include "clock.h"


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/


int main(void) 
{
  //Initialization
  //Enable power interface clock (RM 5.4.15)
  RCC->APBENR1 |= RCC_APBENR1_PWREN;

  /*Peripherals clock enable (RM 5.4.13)*/
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN ; //Enable GPIO A
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN ; //Enable GPIO B
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN ; //Enable GPIO C


  //Clock_InitPll(PLL_40MHZ);
 
  printf("SYSCLK = %lu Hz\n", SystemCoreClock);

  /*Clock output*/
  _GPIO_SetPinMode(GPIOA, 8,  _GPIO_PinMode_AlternateFunction); //
  _GPIO_SetPinAlternateFunction(GPIOA, 8, 0); //AF0 for MCO

  
  RCC->CFGR &= ~(RCC_CFGR_MCOSEL); //Clear MCO bits
  RCC->CFGR |= RCC_CFGR_MCOSEL_1 | RCC_CFGR_MCOSEL_0; //HSI16 selected as MCO source

  /*Prescale MCO output by 16*/
  RCC->CFGR &= ~(RCC_CFGR_MCOPRE); //Clear MCO prescaler bits
  RCC->CFGR |= RCC_CFGR_MCOPRE_2; //MCO prescaler set to 16

  /*Calibrate HSI16*/
  RCC->ICSCR &= ~RCC_ICSCR_HSITRIM_Msk; //Clear current trim
  RCC->ICSCR |= 63 << 8;


  while(1)
  {//Infinite loop.

  }
}




/*************************** End of file ****************************/