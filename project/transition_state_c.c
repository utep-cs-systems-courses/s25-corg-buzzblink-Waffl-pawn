#include <msp430.h>

void transition_state(int state){
     state = (state + 1) % 4;
}
