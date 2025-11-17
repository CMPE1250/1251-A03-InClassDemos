/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include "usart.h"
#include <string.h>


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  int count = 0; //Loop Counter
  char szName[] = "AJ Armstrong";
  char szCourse[] = "CMPE2150";
  char szBuffer[_USART_BUFFSIZE] = {0}; //Standard buffer
  
  //Part A

  //In the one-time initializations section of your code, bring up USART2 at 
    //38400 BAUD. 
  _USART_Init_USART2(SystemCoreClock, 38400);

  //Clear the terminal screen. 
  _USART_ClearScreen(USART2);
  //Print your full name in the terminal aligned to the upper-left corner of a 
    //default 80x24 terminal.
  // Already at home, so...
  _USART_TxString(USART2, szName);
  //Print CMPE1250 aligned to the bottom-right corner of a default 80x24 terminal.
  //_USART_ROWS is declared in my header as 28, _USART_COLS as 80
  _USART_TxStringXY(USART2,_USART_COLS - strlen(szCourse),_USART_ROWS,szCourse);

  _USART_GotoXY(USART2, 2, 2);
  _USART_RxString(USART2, szBuffer, _USART_BUFFSIZE, _USART_RX_ENFORCE_HEX);
  _USART_TxStringXY(USART2, 5, 5, szBuffer);
  
    
  //Event Loop
  for(;;)
  {


    count ++; // Increment loop counter
  }
}

/*************************** End of file ****************************/
