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



  //1. Disable PLL (PLLON in RCC->CR)
  RCC->CR &= ~RCC_CR_PLLON;
  //2. Wait until Pll is fully stopped (PLLRDY in RCC->CR)
  while(RCC->CR & RCC_CR_PLLRDY);
  //3. Write pllRange (N, M, R) and select HSI in RCC->PLLCFGR

  //4. Enable PLL again (PLLON in RCC->CR)
  RCC->CR |= RCC_CR_PLLON;
  //Wait until PLL is locked (PLLRDY in RCC->CR)
  while(!(RCC->CR & RCC_CR_PLLRDY));
  //Enable PLL peripheral and PLLR outputs (PLLPEN and PLLREN in RCC->PLLCFGR)
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN | RCC_PLLCFGR_PLLREN;
  //Set Pll as System Clock (SW_PLL in RCC->CFGR)
  RCC->CFGR |= RCC_CFGR_SW_1; //Set Pll as System Clock

  //Enable MCO line as discussed in class to verify clock speed
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

