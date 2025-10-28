/*********************************************************************
*                  DEMO 7 - Building a USART Library                 *
*                          AJ Armstrong, CNT                         *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <gpio.h>
#include <usart.h>
#include <stdio.h>

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  int count = 0; // Loop Counter
  byte c = 0;

  //User LED (PC6) and PB0 Output
  //Remember that my SetPinMode (and SetAltertnate) start the clocks.
  _GPIO_SetPinMode(GPIOC, 6, _GPIO_PinMode_Output);
  _GPIO_SetPinMode(GPIOB, 0, _GPIO_PinMode_Output);

  //Fire up the main console
  _USART_Init_USART2(SystemCoreClock, 38400);

  for(;;)
  {
    utiDelay(200);
    _USART_TxByte(USART2, '.');
    ++count;
  }
}

/*************************** End of file ****************************/
