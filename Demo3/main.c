/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include "gpio.h"
#include <stm32g031xx.h>

/*********************************************************************
*
*       main()
*
*  Send SOS on the User Led
*   Application entry point.
*/
int main(void) {
 
  // When a button on P is pressed, generate an SOS pattern on the LED.
  // the SOS pattern is 3 dots, 3 dashes, 3 dots, with the length of a dot
  // the same as the gap between symbols, the length of a dash three times
  // that, and the length of 7 dots between them.  So, with each character
  // one time unit, the pattern would be (where _ is off, and - is on):
  // -_-_-_---_---_---_-_-_-_______  before repeating (30 symbols total).

  int count = 0; //Loop Counter
  int waiting = 1; //Am I waiting for the button to press?

  //Set up PC6 (LED) as output, PB1 as Input
  //Set up the ports
  _GPIO_ClockEnable(GPIOC);
  _GPIO_ClockEnable(GPIOB);

  //Set up the pins
  _GPIO_SetPinMode(GPIOC, 6, _GPIO_PinMode_Output);
  //Don't forget to use a pull-down (or pull-up) on the pin
  _GPIO_SetPinMode(GPIOB, 1, _GPIO_PinMode_Input);

  for (;;)
  {
    count++; //Increment my loop counter    
    utiDelay(300); // My busy wait loop delay.  This is the length of a dot

    // Just going to sit here and spin until the input pin goes high...
    // but if I'm not waiting, I'll skip this line
    while(waiting && !_GPIO_GetPinIState(GPIOB, 1));
    //If I make it here, someone pressed the button!
    //My wait is over...
    if (waiting) count = 0;  //Before I stop waiting, start over at 0 count
    waiting = 0;
    //Now I start on my tick count (NB: This could also just be a simple for loop with
    //if stack.  I like the compressed syntax of switches and the utility of fall-through.)
    switch(count % 30)
    {
      case 0: _GPIO_PinSet(GPIOC, 6); break;
      case 1: _GPIO_PinClear(GPIOC, 6); break;
      case 2: _GPIO_PinSet(GPIOC, 6); break;
      case 3: _GPIO_PinClear(GPIOC, 6); break;
      case 4: _GPIO_PinSet(GPIOC, 6); break;
      case 5: _GPIO_PinClear(GPIOC, 6); break;
      case 6: case 7: case 8: _GPIO_PinSet(GPIOC, 6); break;
      case 9: _GPIO_PinClear(GPIOC, 6); break;
      case 10: case 11: case 12: _GPIO_PinSet(GPIOC, 6); break;
      case 13: _GPIO_PinClear(GPIOC, 6); break;
      case 14: case 15: case 16: _GPIO_PinSet(GPIOC, 6); break;
      case 17: _GPIO_PinClear(GPIOC, 6); break;
      case 18: _GPIO_PinSet(GPIOC, 6); break;
      case 19: _GPIO_PinClear(GPIOC, 6); break;
      case 20: _GPIO_PinSet(GPIOC, 6); break;
      case 21: _GPIO_PinClear(GPIOC, 6); break;
      case 22: _GPIO_PinSet(GPIOC, 6); break;
      case 23: case 24: case 25: case 26: case 27: case 28: _GPIO_PinClear(GPIOC, 6); break;
      //29 (my 30th segment) is the end, so we'll also set things back to wait for a button.
      case 29: _GPIO_PinClear(GPIOC, 6); waiting = 1; break;
    } 
  }
}

/*************************** End of file ****************************/
