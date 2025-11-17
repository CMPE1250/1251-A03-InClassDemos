/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stm32g031xx.h>
#include "gpio.h"

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  int count = 0;
  int invert = 0;

  //Set up ports
  //Outputs PB0,PA5,PB8
  //Input PB2
  _GPIO_ClockEnable(GPIOA);
  _GPIO_ClockEnable(GPIOB);

  _GPIO_SetPinMode(GPIOA, 5, _GPIO_PinMode_Output);
  _GPIO_SetPinMode(GPIOB, 0, _GPIO_PinMode_Output);
  _GPIO_SetPinMode(GPIOB, 8, _GPIO_PinMode_Output);
  _GPIO_SetPinMode(GPIOB, 2, _GPIO_PinMode_Input);

    //Test if my two pins are jumpered
    if(_GPIO_GetPinIState(GPIOB, 2) == _GPIO_GetPinOState(GPIOB, 8))
    {
      _GPIO_PinToggle(GPIOB, 2);
      if(_GPIO_GetPinIState(GPIOB, 2) == _GPIO_GetPinOState(GPIOB, 8))
        _GPIO_PinClear(GPIOA, 5);
      else
        _GPIO_PinSet(GPIOA, 5);
    }
    else
       _GPIO_PinSet(GPIOA, 5);
  _GPIO_PinSet(GPIOA, 5);
  _GPIO_PinClear(GPIOB, 0);
  _GPIO_PinClear(GPIOB, 8);

  for (;;)
  {
    utiDelay(50); //Time base
    _GPIO_PinToggle(GPIOB, 0);
    
  //Test if my two pins are jumpered
    if(_GPIO_GetPinIState(GPIOB, 2) == _GPIO_GetPinOState(GPIOB, 8))
    {
      _GPIO_PinToggle(GPIOB, 2);
      if(_GPIO_GetPinIState(GPIOB, 2) == _GPIO_GetPinOState(GPIOB, 8))
        invert = 1;
      else
        invert = 0;
    }
    else
        invert = 0;
    
    if (count%3 == 0) invert?_GPIO_PinSet(GPIOA, 5):_GPIO_PinClear(GPIOA, 5);
    else if (count%3 == 1) invert?_GPIO_PinSet(GPIOA, 5):_GPIO_PinClear(GPIOA, 5);
    else if (count%3 == 2) invert?_GPIO_PinClear(GPIOA, 5):_GPIO_PinSet(GPIOA, 5);

    count++; //Loop counter.
    

  }

}

/*************************** End of file ****************************/
