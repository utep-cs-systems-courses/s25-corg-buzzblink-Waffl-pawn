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
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state var to count time
int secondCount = 0;
int state = 0;

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  secondCount++;

  if(state >= 1){
    if(secondCount >= 1)
      P1OUT ^= LED_RED;
  }
  else if(state >= 2){
    if(secondCount <= 50)
      P1OUT ^= LED_RED;
  }
  else if(state >= 3){
    if(secondCount <= 150)
      P1OUT ^= LED_RED;
  }

  if(secondCount >= 250) { 	/* once each sec... */
    secondCount = 0;/* reset count */
    state++;
    if(state >= 3)
      state = 0;
    P1OUT ^= LED_GREEN;/* toggle green LED */
  }
  
} 

