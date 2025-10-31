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

//Constants

#define BUFFSIZE 5

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  int count = 0; // Loop Counter
  int delay = 10000; //How many loops to wait before the next event
  byte c = 0;
  int bufindx = 0; // Current index of my circular buffer
  char buffer[BUFFSIZE] = {0};
  int charcount = 0;
  int avealpha = 0;
  int sumbuffer = 0;

  //User LED (PC6) and PB0 Output
  //Remember that my SetPinMode (and SetAltertnate) start the clocks.
  _GPIO_SetPinMode(GPIOC, 6, _GPIO_PinMode_Output);
  _GPIO_SetPinMode(GPIOB, 0, _GPIO_PinMode_Output);

  //Fire up the main console
  _USART_Init_USART2(SystemCoreClock, 38400);

  //Average value of lower case ascii
  avealpha = ('z'+'a')/2;

  for(;;)
  {
    if(!(count%delay))
      _USART_TxByte(USART2, '.'); 
    if (_USART_RxByte(USART2, &c))
    {
      //increment character counter
       charcount++;
       printf("%c",c);
      _USART_TxByte(USART2, c);
      //PART D - Circular buffer
      //Write my new character to my next pos'n
      buffer[bufindx] = c;
      //Figure out next index
      bufindx = (bufindx + 1) % BUFFSIZE;
      if(charcount >= 5)
      {
        //If the ASCII sum of the characters is above or equal to five times the 
        //average ASCII value of the lowercase alphabet, turn on the green user 
        //LED (PC6), otherwise turn it off.
        sumbuffer=0;
        for (int i=0; i < BUFFSIZE ; ++i) sumbuffer += buffer[i];
        if(sumbuffer>avealpha*5)
          _GPIO_PinSet(GPIOC, 6);
        else
          _GPIO_PinClear(GPIOC, 6);
      }
    }
    if(USART2->ISR & USART_ISR_ORE) //Overrun Error flag sit
    {
      printf("\nOverrun!\n");
      //Now, handle the error by simply accepting the loss of the character
      //If there is no data to actually read, clear the error
      if (!(USART2->ISR & USART_ISR_RXNE_RXFNE)) // No data
          //Clear the Overrun Error (dropping lost bytes)
          USART2->ICR |= USART_ICR_ORECF;
    }
    ++count;
  }
}


/*************************** End of file ****************************/
