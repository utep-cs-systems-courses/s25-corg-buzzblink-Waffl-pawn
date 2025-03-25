
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

static char
switch_update_interrupt_sense(){
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */
  return p2val;
}

void configure_buttons(){
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */

  switch_update_interrupt_sense();
}


int switches = 0;

void
switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

void __interrupt_vec(WDT_VECTOR) WDT(){
  static int blink_count = 0;
  static int cycle =500;
  blink_count++;
  
  switch(state){
  case 0://turning everything off
    P1OUT &= ~LEDS;
    buzzer_set_period(0);
    break;
  case 1://turning on green and increasing buzzer cycle
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_RED;
    buzzer_set_period(cycle++);
    break;
  case 2://turning on red and decreasing buzzer cycle
    P1OUT |= LED_RED;
    P1OUT &= ~LED_GREEN;
    buzzer_set_period(cycle--);
    break;
  case 3://Dimming the lights... because they're too BRIGHT!!
    if(blink_count >= 1){
      P1OUT |= LED_GREEN;
      P1OUT |= LED_RED;
    }else{
      P1OUT &= ~LED_GREEN;
      P1OUT &= ~LED_RED;
    }  

    //turning buzzer off and on
    if(blink_count >= 150){
      buzzer_set_period(0);
    }else
      buzzer_set_period(500);
   
    if(blink_count >= 250) blink_count=0;
    break;
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & SW1){
    state = 0;
    //transition_state(state);
    P2IFG &= ~BIT0;
    switch_interrupt_handler();
  }
  if(P2IFG & SW2){
    state = 1;
    P2IFG &= ~BIT1;
    switch_interrupt_handler();
  }
  if(P2IFG & SW3){
    state = 2;
    P2IFG &= ~BIT2;
    switch_interrupt_handler();
  }
  if(P2IFG & SW4){
    state = 3;
    P2IFG &= ~BIT3;
    switch_interrupt_handler();
  }
  
}

