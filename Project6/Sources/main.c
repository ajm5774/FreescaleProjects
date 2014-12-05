#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

const int BUTTONID = 5; 
servo theServo = {0,0,0,0,0};

void main(void) {
  float input;
  waitForButtonPushed(BUTTONID);
  
  if(!ack())
     theServo->leds &= LED_0_ON;
  
  while(true) {
      input = readInput();
      mov(theServo, input); 
  }
}

void waitForButtonPushed(int buttonId) {
  
  
}

int ack() {
  
}

float readInput() {
    
}
