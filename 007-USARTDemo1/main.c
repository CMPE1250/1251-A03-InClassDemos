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

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  int count = 0; // Loop Counter

  //Fire up the main console
  _USART_Init_USART2(SystemCoreClock, 38400);

  for(;;)
  {

    ++count;
  }
}

/*************************** End of file ****************************/
