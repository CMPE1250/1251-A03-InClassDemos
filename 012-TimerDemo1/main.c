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


  Clock_InitPll(PLL_40MHZ);
 
  printf("SYSCLK = %lu Hz\n", SystemCoreClock);

  _GPIO_SetPinMode(GPIOB, 3,  _GPIO_PinMode_Output);

  /*Disable timer, in case already enabled*/
  TIM14->CR1 &= ~TIM_CR1_CEN;
  /*Set prescaler to 40-> TIck @ 1[MHz]*/
  TIM14->PSC = 40 - 1;
  /*Set Autoreload in ARR register*/
  TIM14->ARR = 250; //ticks in [us]
  /*Enable counter*/
  TIM14->CR1 |= TIM_CR1_CEN;
  
  while(1)
  {//Infinite loop.

    /*Experimentation
    - Insert a blocking delay here
    - See what happens
    - See how you can fix it (ISR?)
    */
    //Poll flag
    /*Check if the flag is active, otherwise , skip through*/
     if(TIM14->SR & TIM_SR_UIF)
     {
       _GPIO_PinToggle(GPIOB, 3);
       TIM14->SR &= ~TIM_SR_UIF;//Clear flag
     }

    //BLock code, different approach
    /*Block the code until the flag is active*/
    //while(!(TIM14->SR & TIM_SR_UIF));
    //_GPIO_PinToggle(GPIOB, 3);
    //TIM14->SR &= ~TIM_SR_UIF;//Clear flag 
  }
}




/*************************** End of file ****************************/