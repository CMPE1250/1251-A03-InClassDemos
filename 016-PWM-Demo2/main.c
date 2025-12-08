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
  uint16_t msCounter = 0;
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

  /*Set PA7 to alternate function TIM14->CH1*/
  _GPIO_SetPinMode(GPIOA, 7,  _GPIO_PinMode_AlternateFunction);
  _GPIO_SetPinAlternateFunction(GPIOA, 7, 4); //AF4 for TIM14_CH1
  _GPIO_SetPinMode(GPIOB, 3,  _GPIO_PinMode_Output);

  /*Disable timer, in case already enabled*/
  TIM14->CR1 &= ~TIM_CR1_CEN;
  /*Set prescaler to 40-> TIck @ 1[MHz]*/
  TIM14->PSC = 40 - 1;
  /*Set Autoreload in ARR register: period*/
  TIM14->ARR = 1000 - 1; //ticks in [us] //mod-1000 counter
  /*POsitive duty*/
  TIM14->CCR1 = 500; //500 ticks -> 500[us] -> 50% DC

  /*Enable TIM14 CH1*/
  TIM14->CCER |= TIM_CCER_CC1E;

  /*PWM Mopde 1 for CCR1*/
  TIM14->CCMR1 &= ~TIM_CCMR1_OC1M;//Clear Output comare mode bits
  TIM14->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
  //TIM14->CCMR1 |= 0b110 << 4;  //same as above

  /*Enable counter*/
  TIM14->CR1 |= TIM_CR1_CEN;


  /*Enable switches*/
  _GPIO_SetPinMode(GPIOB, 4,  _GPIO_PinMode_Input); //Switch 1
  _GPIO_SetPinMode(GPIOB, 3, _GPIO_PinMode_Input); //Switch 2

  /*Enable built in LED*/
  _GPIO_SetPinMode(GPIOC, 6,  _GPIO_PinMode_Output); //Built in LED
  
  while(1)
  {//Infinite loop.
    //Check for timer update event
    if(TIM14->SR & TIM_SR_UIF)
    {//THis will happen every 1[ms]
      //Clear flag
      TIM14->SR &= ~TIM_SR_UIF;
      //Toggle LED
      _GPIO_PinToggle(GPIOC,6);
      //Increment ms counter
      if(++msCounter >= 25)
      {
        msCounter = 0;
        //Check switches every 1[ms]
        if(!_GPIO_GetPinIState(GPIOB,3))
        {//RIGHT switch pressed
          //Increase duty cycle
          if(TIM14->CCR1 < 990)
          {
            TIM14->CCR1 += 10;
          }
        }
        else
        {//RIGHT Switch not pressed
          
        }
        if(!_GPIO_GetPinIState(GPIOB,4))
        {//LEFT switch pressed
          //Decrease duty cycle
          if(TIM14->CCR1 > 10)
          {
            TIM14->CCR1 -= 10;
          }
        }
        else
        {//LEFT Switch not pressed
          
        }        
      }
    }
  }
}




/*************************** End of file ****************************/