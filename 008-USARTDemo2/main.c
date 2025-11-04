/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include "util.h"
#include "gpio.h"
#include "usart.h"


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  int count = 0; //Loop Counter

  _USART_Init_USART2(SystemCoreClock, 38400);

  _USART_TxStringXY(USART2, 7, 7, "AJ ARMSTRONG");
  
  //Part A

  //In the one-time initializations section of your code, bring up USART2 at 
    //38400 BAUD. 

  //Clear the terminal screen.
  //Print your full name in the terminal aligned to the upper-left corner of a 
    //default 80x24 terminal.
  //Print CMPE1250 aligned to the bottom-right corner of a default 80x24 terminal.

    
  //Event Loop
  for(;;)
  {

    count ++; // Increment loop counter
  }
}

/*************************** End of file ****************************/
