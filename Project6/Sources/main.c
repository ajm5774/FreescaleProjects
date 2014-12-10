#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "types.h"
#include "servo.h"
#include "button.h"

// Definitions
#define BUS_CLK_FREQ  ((UINT32)2000000)
#define PERIODMILLIS  ((UINT16)20)
#define PWMPRCLKVALUE ((UINT16)4)
#define PWMSCLAVALUE  ((UINT16)5)

INT32 clkafreq;
const int BUTTONID = 5; 
Servo theServo = { 0, 0, &PWMDTY0, &PWMPER0, &PORTB };
byte inputByte = 0;
int hasInput = 0;


void InitializeTimer(void)
{
  TIE_C1I = 0;//make sure the timer is disabled at the start

  // Set the timer prescaler to %2, since the bus clock is at 2 MHz,
  // and we want the timer running at 1 MHz
  TSCR2_PR0 = 1;
  TSCR2_PR1 = 0;
  TSCR2_PR2 = 0;
    
  //Capture input on falling edge only
  TCTL4_EDG1B = 1;
  TCTL4_EDG1A = 0;
  
  // Enable the timer
  TSCR1_TEN = 1;

  // Enable interrupts via macro provided by hidef.h
  EnableInterrupts;
}

void ConfigurePer(UINT16 periodMillis, Servo *servo ) 
{
   * servo->pwmPer = clkafreq *  periodMillis / 1000;
}

void InitializePwm(void) {
  //enable pulse width modulation
  //PWME = 1;
  PWME_PWME0 = 1;
  
  //Set the polarity
  //PWMPOL = 1;
  PWMPOL_PPOL0 = 1;
  
  //Enabling the PWM clock
  //PWMCLK = 1;
  PWMCLK_PCLK0 = 1;
  
  //choose PWM clock
  PWMPRCLK = PWMPRCLKVALUE;
  
  //scale PWM clock
  PWMSCLA = PWMSCLAVALUE;
  
  clkafreq = BUS_CLK_FREQ / (PWMSCLAVALUE * 2);
  clkafreq = clkafreq / (1 << PWMPRCLKVALUE);
  ConfigurePer(PERIODMILLIS, &theServo);
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
         
  //read input
  //inputByte = inputByte << 1 & TC1;
  //inputByte = inputByte << 1 & TC2;
  //inputByte = inputByte << 1 & TC3;
  //inputByte = inputByte << 1 & TC4;
  //inputByte = inputByte << 1 & TC5;
  
  //mark that we have input
  hasInput = 1;
}
#pragma pop


int ack() {
   return 0;
}


void main(void) {
  /*waitForButtonPushed();
  if(!ack())
      theServo->leds &= LED_0_ON;

  while(true) {
    if(hasInput) {
      if(!ack())
       theServo->leds &= LED_0_ON;
      mov(theServo, inputByte);
      inputIndex = 0;//reset input index
      hasInput = 0;  //reset input flag
    }
  } */
  
  InitializeTimer(); 
  InitializePwm();
  
  while(1) {
    if(hasInput) {
      mov(&theServo, 2);
      hasInput = 0;  //reset input flag
    } else {
        mov(&theServo, 0);
    }
  }
}



