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
  RCC->APBENR2 |= RCC_APBENR2_TIM14EN; //Enable timer 14

  /*Define HCLK clock ratio to the FLASH,
  slow down system to access flash (RM 3.7.1)*/
  FLASH->ACR |= FLASH_ACR_PRFTEN_Msk;     //Enable instruction prefetch  
  FLASH->ACR |= FLASH_ACR_LATENCY_1; // Set 2 wait states


  /*Calibrate HSI16 - Add thisd to your library later*/
  RCC->ICSCR &= ~RCC_ICSCR_HSITRIM_Msk; //Clear current trim
  RCC->ICSCR |= 63 << 8;
  Clock_InitPll(PLL_40MHZ);
 
  printf("SYSCLK = %lu Hz\n", SystemCoreClock);

  _GPIO_SetPinMode(GPIOB, 3,  _GPIO_PinMode_Output);

  /*Disable timer, in case already enabled*/
  TIM14->CR1 &= ~TIM_CR1_CEN;
  /*Set prescaler to 40-> TIck @ 1[MHz]*/
  TIM14->PSC = 40 - 1;
  /*Set Autoreload in ARR register*/
  TIM14->ARR = 250 - 1; //ticks in [us] //mod-250 counter

  /*OUTPUT COMPARE SETTINGS*/
  TIM14->CCR1 = 50; //50 ticks -> 50[us]



  /*Enable counter*/
  TIM14->CR1 |= TIM_CR1_CEN;
  
  while(1)
  {//Infinite loop.
     if(TIM14->SR & TIM_SR_UIF)
     {
       _GPIO_PinSet(GPIOB, 3);
       TIM14->SR &= ~TIM_SR_UIF;//Clear flag
     }
     if(TIM14->SR & TIM_SR_CC1IF)
     {
       _GPIO_PinClear(GPIOB, 3);
       TIM14->SR &= ~TIM_SR_CC1IF;//Clear flag
     }
  }
}




/*************************** End of file ****************************/