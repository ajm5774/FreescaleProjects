#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

const int BUTTONID = 5; 
servo theServo = { 0, 0, &PWMDTY0, &PWMPER0, &PORTB };
byte inputByte = 0;
int inputIndex = 0;
int hasInput = 0;


void InitializeTimer(void)
{
  TIE_C1I = 0;//make sure the timer is disabled at the start

  // Set the timer prescaler to %2, since the bus clock is at 2 MHz,
  // and we want the timer running at 1 MHz
  TSCR2_PR0 = 1;
  TSCR2_PR1 = 0;
  TSCR2_PR2 = 0;
    
  //Capture input on rising edge only
  TCTL4_EDG1B = 1;
  TCTL4_EDG1A = 1;
  
  // Enable the timer
  TSCR1_TEN = 1;

  // Enable interrupts via macro provided by hidef.h
  EnableInterrupts;
}


//Input capture. Captures the current time, quick calculates the time
//then increments the occurences of the time in the measurements array
#pragma push
#pragma CODE_SEG __SHORT_SEG NON_BANKED
//--------------------------------------------------------------       
void interrupt 9 OC1_isr( void )
{
  //clear the interrupt flag   
  TFLG1   =   TFLG1_C1F_MASK;
  
  inputByte = inputByte << 1 & TC1;
  
  if(++inputIndex >= 7) {
    hasInput = 1;
  }
}
#pragma pop


int ack() {
  
}


void main(void) {
  waitForButtonPushed();
  
  if(!ack())
     theServo->leds &= LED_0_ON;
  
  while(true) {
    if(hasInput) {
      mov(theServo, inputByte);
      inputIndex = 0;//reset input index
      hasInput = 0;  //reset input flag
    }
  }
}



