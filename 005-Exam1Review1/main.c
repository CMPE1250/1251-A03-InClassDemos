/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Practice Exam Question 1 Solution

*/

#include <stm32g031xx.h>
#include "gpio.h"
#define SUCCESS 0

/*********************************************************************
*
*       AJ's QDSolution to Practice Question 1
*/
int main(void) {
  int count = 0; // Loop counter
  uint pulses = 0; // Number of 10 ms pulses to send

  //Set up my clocks 
  _GPIO_ClockEnable(GPIOA);
  _GPIO_ClockEnable(GPIOB);

  //Set up my inputs (don't forget the pull-downs!)
  _GPIO_SetPinMode(GPIOA,9,_GPIO_PinMode_Input);
  _GPIO_SetPinMode(GPIOB,0,_GPIO_PinMode_Input);
  _GPIO_SetPinMode(GPIOB,2,_GPIO_PinMode_Input);

  //And my output (PA5)
  _GPIO_SetPinMode(GPIOA, 5, _GPIO_PinMode_Output);

  //My time base is 200ms with 10ms chunks
  for(;;)
  {
    count++;
    utiDelay(10); //This is my basic smallest event length.~~``~~`-
    //Every 200 ms, I want to read my three bit number.
    //One pulse is actually 2 transitions, so I need twice
    //as many. Only read every 200 ms, not every loop
    if(!(count%20)) //Now, build my number from the three bits
      pulses = 2 *(_GPIO_GetPinIState(GPIOA, 9) << 2 |
                   _GPIO_GetPinIState(GPIOB, 0) << 1 |
                   _GPIO_GetPinIState(GPIOB, 2));
    //For the next 'pulses' loops, I want to togle A5
    if (pulses) 
    {
      _GPIO_PinToggle(GPIOA, 5);
      pulses--;
    }
  }

  return SUCCESS;
}

/*************************** End of file ****************************/
