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

  //Fire up the main console
  _USART_Init_USART2(SystemCoreClock, 38400);

  for(;;)
  {
    utiDelay(500);
    _USART_TxByte(USART2, 'A');
    if(_USART_RxByte(USART2, &c))
      printf("%c...",c);
    ++count;
  }
}


/*************************** End of file ****************************/
