#include <msp430.h>
#include "buzzer.h"
#include "led.h"
#include "libTimer.h"

#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)

void configure_buttons();
void state_machine();
void transition_state(int state);

static char state = 0;
static char brightness = 0;

void main(void){
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;

  configureClocks();
  enableWDTInterrupts();

  configure_buttons();
  buzzer_init();

  or_sr(0x18);
}

void configure_buttons(){
  P2REN |= SWITCHES;
  P2OUT |= SWITCHES;
  P2IE = SWITCHES;
  P2DIR &= ~SWITCHES;

  P2IES |= (P2IN & SWITCHES);
  P2IES &= (P2IN | ~SWITCHES);
}

void __interrupt_vec(WDT_VECTOR) WDT(){
  static int blink_count = 0;
  buzzer_set_period(0);
  
  switch(state){
  case 0:
    P1OUT &= ~LEDS;
    buzzer_set_period(300);
    break;
  case 1:
    P1OUT |= LED_RED;
    P1OUT &= ~LED_GREEN;
    buzzer_set_period(900);
    break;
  case 2:
    if(++blink_count >= 125){
      P1OUT |= LED_GREEN;
    }else
      P1OUT &= ~LED_GREEN;

    brightness = (brightness + 1) % 20;
    buzzer_set_period(950);
    break;
  }
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void){
  if(P2IFG & SW1){
    state = 1;
    //transition_state(state);
    P2IFG &= ~BIT0;
  }
  if(P2IFG & SW2){
    state = 2;
    P2IFG &= ~BIT1;
  }
  if(P2IFG & SW3){
    state = 3;
    P2IFG &= ~BIT2;
  }
  if(P2IFG & SW4){
    state = 0;
    P2IFG &= ~BIT3;
  }
  
}
