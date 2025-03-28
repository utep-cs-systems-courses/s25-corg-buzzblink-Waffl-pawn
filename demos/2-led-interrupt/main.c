//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}


void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  static int count = 0;
  static int stored = 0;
  count++; 

  if (count >= 250) {
    if (stored == 0) {
      P1OUT |= LED_GREEN;  
      P1OUT &= ~LED_RED;   
      stored = 1;
    } else {
      P1OUT |= LED_RED;
      P1OUT &= ~LED_GREEN;
      stored = 0;
    }
    count = 0;
  }
}
 
