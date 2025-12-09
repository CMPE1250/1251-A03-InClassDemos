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
  uint8_t leftState = 0, rightState = 0, centerState = 0, pwmMode = 0;






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

  /*PWM Mode 1 for CCR1*/
  TIM14->CCMR1 &= ~TIM_CCMR1_OC1M;//Clear Output comare mode bits
  TIM14->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
  //TIM14->CCMR1 |= 0b110 << 4;  //same as above

  /*Enable counter*/
  TIM14->CR1 |= TIM_CR1_CEN;


  /*Enable switches*/
  _GPIO_SetPinMode(GPIOB, 4,  _GPIO_PinMode_Input); //Switch 1
  _GPIO_SetPinMode(GPIOB, 3, _GPIO_PinMode_Input); //Switch 4
    _GPIO_SetPinMode(GPIOB, 5, _GPIO_PinMode_Input); //Switch 2 - center

  /*Enable built in LED*/
  _GPIO_SetPinMode(GPIOC, 6,  _GPIO_PinMode_Output); //Built in LED
  
  while(1)
  {//Infinite loop.
    /*Check for RIGHT Button*/
    if(!_GPIO_GetPinIState(GPIOB,3))
    {//RIGHT switch pressed
      if(!leftState)
      {//Only act on the transition
        leftState = 1;
        //Increase duty cycle
        if(TIM14->CCR1 < 950)
        {
          TIM14->CCR1 += 50;
        }
      }
    }
    else
    {//RIGHT Switch not pressed
      if(leftState)
      {
        leftState = 0;
      }
    }
    /*Check for LEFT Button*/
    if(!_GPIO_GetPinIState(GPIOB,4))
    {//LEFT switch pressed
      if(!rightState)
      {
        rightState = 1;
        //Decrease duty cycle
        if(TIM14->CCR1 > 50)
        {
          TIM14->CCR1 -= 50;
        }
      }
    }
    else
    {//LEFT Switch not pressed
      if(rightState)
      {
        rightState = 0;
      }      
    }  
    //  /*Check for CTR Button*/
    if(!_GPIO_GetPinIState(GPIOB,5))
    {//CTR switch pressed
      if(!centerState)
      {
        centerState = 1;
      }
    }
    else
    {//CTR Switch not pressed
      if(centerState)
      {
        centerState = 0;
        /*swap PWM mode*/
        pwmMode ^= 1;  
        if(pwmMode)
        {//If pwmMode == 1 -> PWM2
          /*PWM Mode 2*/
          TIM14->CCMR1 &= ~TIM_CCMR1_OC1M;//Clear Output comare mode bits
          TIM14->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
        }
        else
        {//If pwmMode == 0 -> PWM1
          /*PWM Mode 1*/
          TIM14->CCMR1 &= ~TIM_CCMR1_OC1M;//Clear Output comare mode bits
          TIM14->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
        }
      }      
    }   
  }
}




/*************************** End of file ****************************/