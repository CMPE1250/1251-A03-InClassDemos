/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stm32g031xx.h>


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
  int count = 0; //Loop Counter
  unsigned int temp = 0; // Intermediate scratch for editing registers

  //Demonstrating the basic steps to set the System Clock to 40MHz (from 16MHz)

  //After doing my research, I've determined that, for 40 MHz via PLL R:
  // (a) - 5.2.8 - I need voltage range 1 to go above 16MHz
  // (b) - 3.3.4 - In voltage range 1, I need 1 wait state for FLASH read latency
  // (c) - 5.4.4 - fvco = fpllin x (N/M) = 16MHz (HSI) x (N/M)
  //                To get to 40 MHz, I want fvco to be a multiple of 40 (80, 120, ...)
  //                80 is pretty doable as 16/2 X 10 -> N=10, M=5. (Check 5.4.4 for allowable ranges)
  //                To get 40, we just set the R divisor to 2. 

  // (1) 4.4.1 - Configure the voltage range to set voltage range 1. NB: THE VOS FIELD DOESN'T PERMIT
  //     WRITING 00 or 11. So we will just use a temporary variable to write it all at once.
  temp = PWR->CR1; //Get the current state
  temp &= ~PWR_CR1_VOS_Msk; //Clear the bits in my copy
  temp |= 01 << PWR_CR1_VOS_Pos; //Write 01 (Power Range 1) into the field
  PWR->CR1 = temp; // Set/unset all bits in one operation.
  //*** 4.4.6 Wait for VOSF to clear by watching VOSF in status register two, until it is 0
  //    This lets us know our voltage scaling is complete.
  while(PWR->SR2 & PWR_SR2_VOSF_Msk){;;} //Loop until VOSF is a zero

  // (2) 3.7.1 - Set Flash wait states (latency)
  FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk; //Clear existing bits
  FLASH->ACR |= 0b001 << FLASH_ACR_LATENCY_Pos; // (1 latency) is 001 - 3.7.1
  // 3.7.1 for this field says "a new write becomes effective when it returns the same value upon read"
  // so, let's wait until we can read that same value back...
  while((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != 0b001 << FLASH_ACR_LATENCY_Pos) {;;} //Wait for it to set

  // (3) Before I configure the PLL, I need to make sure I'm not already using it. I also need to make
  //     sure the HSI16 clock source is up and running, because I will use that for the PLL input
  //     source.
  // 5.4.1 First, we'll fire up the HSI and make sure it's stable
  RCC->CR |= RCC_CR_HSION_Msk; // Turn on HSI
  while(!(RCC->CR & RCC_CR_HSIRDY_Msk)) {;;} //Wait for it to be ready
  // 5.4.3 Move the system clock over to HSI16, in case it is currently pointed at the PLL
  RCC->CFGR &= ~RCC_CFGR_SW_Msk; //Clear the existing field
  RCC->CFGR |= 0b000 << RCC_CFGR_SW_Pos; //5.4.3: 000 is HSISYS
  // Wait to confirm that we are using HSI ---See the notes for SWS in 5.4.3
  while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0b000 << RCC_CFGR_SWS_Pos)) {;;}
 
  // (4) 5.4.1 Now that we're sure we're not using the PLL for our system clock, 
  //     shut it down for configuration
  RCC->CR &= ~RCC_CR_PLLON_Msk;
  //Wait to confirm it is properly stopped.
  while(RCC->CR & RCC_CR_PLLRDY_Msk){;;} //A 1 in this bit means locked and ready for use, we want 0
  
  // (5) 5.4.4 We can finally input our settings into the PLL configuration register.  I'm just going to
  //     construct all the values.  Although I don't care about P and Q clocks right now, I'm going to
  //     disable them and put some typical values in, because I'm rewriting the whole register rather
  //     that one field at a time.  Other than R, important settings here are toward the end: N, M, and 
  //     the selection of HSI as the source clock. See individual field descriptions for values.
  temp =    0b001 << RCC_PLLCFGR_PLLR_Pos   | //R is /2 (Take the fvco (80MHz) and divide by 2)
              0b0 << RCC_PLLCFGR_PLLREN_Pos | //Disable R (for now, will enable after this configuration)
            0b011 << RCC_PLLCFGR_PLLQ_Pos   | //Q is /4
              0b0 << RCC_PLLCFGR_PLLQEN_Pos | //Disable Q
          0b00001 << RCC_PLLCFGR_PLLP_Pos   | //P is /2 
              0b0 << RCC_PLLCFGR_PLLPEN_Pos | //Disable P 
        0b0001010 << RCC_PLLCFGR_PLLN_Pos   | //N is X10 (This would set my fvco to 160MHz, except...)
            0b000 << RCC_PLLCFGR_PLLM_Pos   | //M is /2 (this subsequently divides it by 2 for 80MHz)
             0b10 << RCC_PLLCFGR_PLLSRC_Pos;  //HSI16 src
  RCC->PLLCFGR = temp; // Write everything at once (only do this if happy changing EVERY field in the register
  // Enable PLL R now that configuration is complete.
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN_Msk;

  // (6) 5.4.1 OK.  The PLL is now configured for a 40 MHz PLLRCLK.  We can turn it on, wait for
  //     it to be stable, then shift over to using it as my source clock.
  RCC->CR |= RCC_CR_PLLON_Msk; // Turn on the PLL
  while(!(RCC->CR & RCC_CR_PLLRDY_Msk)){;;} // Wait for the PLL to be ready (RCC_CR_PLLRDY)
  // 5.4.3 Use PLLR as the clock source for SYSCLK
  RCC->CFGR &= ~RCC_CFGR_SW_Msk;
  RCC->CFGR |= 0b010 << RCC_CFGR_SW_Pos; //5.4.3: 010 is PLLRCLCK
  //wait to be sure it took (this is another of those fields you can read back to confirm)
  while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0b010 << RCC_CFGR_SWS_Pos)) {;;}
  //Reset System Core Clock value so other functions using it don't break.
  //SystemCoreClock = 40000000;
  SystemCoreClockUpdate(); //USING CMSIS, BAD AJ?
  printf("System core clock = %u",SystemCoreClock);

  //Event Loop
  for(;;)
  {
    //Do whatever you want, it'll be faster!

    count ++; // Increment loop counter
  }
}

/*************************** End of file ****************************/
