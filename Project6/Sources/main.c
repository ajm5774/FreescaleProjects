#include <hidef.h>      /* common defines and macros */
#include <stdio.h>      /* Standard I/O Library */
#include "derivative.h"      /* derivative-specific definitions */
#include "types.h"
#include "servo.h"
#include "button.h"

// Definitions
#define BUS_CLK_FREQ  ((UINT32)2000000)
#define PERIODMILLIS  ((UINT16)20)
#define PWMPRCLKVALUE ((UINT16)4)
#define PWMSCLAVALUE  ((UINT16)5)

#define TC1_VAL       ((UINT16)  (((BUS_CLK_FREQ / PRESCALE) / 2) / OC_FREQ_HZ))
#define PRESCALE      ((UINT16)  2)


INT32 clkafreq;
const int BUTTONID = 5; 
Servo theServo = { 0, 0, &PWMDTY0, &PWMPER0, &PORTB };
byte inputByte = 0;
int readInput = 0;
int parityBit = 0;
int dataLength = 5;
int setInput = 0;
int firstInput = 1;

void InitializeSerialPort(void)
{
    // Set baud rate to ~9600 (See above formula)
    SCI0BD = 13;          
    
    // 8N1 is default, so we don't have to touch SCI0CR1.
    // Enable the transmitter and receiver.
    SCI0CR2_TE = 1;
    SCI0CR2_RE = 1;
}

void InitPorts(void) 
{
   DDRA = 0x80;//1000 0000  output channel 8
   DDRB = 0xff;//1000 0000  output channel 8
}


void InitializeTimer(void)
{
  TIE_C1I = 0;//make sure the timer is disabled at the start

  // Set the timer prescaler to %2, since the bus clock is at 2 MHz,
  // and we want the timer running at 1 MHz
  TSCR2_PR0 = 1;
  TSCR2_PR1 = 0;
  TSCR2_PR2 = 0;
  
  //Sets all timers to input capture
  TIOS = 0;
    
  //Capture input on falling edge only
  TCTL4_EDG1B = 1;
  TCTL4_EDG1A = 1;
  
  // Enable the timer
  TSCR1_TEN = 1;

  // Enable interrupts via macro provided by hidef.h
  EnableInterrupts;
  TIE_C1I = 1;
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
  
  //mark that we have input
  readInput = !readInput;
  
  if(readInput) {
    //read input
    inputByte = PORTA & 0x1f;
    
    parityBit = PORTA >> 5 & 1;
    
    PORTA_BIT7 = 1;//ack up
    
    setInput = 1;
    
    if(inputByte)
      firstInput = 0;
  }
  else {
    inputByte = 0;
    PORTA_BIT7 = 0;//ack down
  }
  
}
#pragma pop

int checkParity() 
{
	int i;
	int parityCalc = 0;

	for(i = 0; i < dataLength; i++)
	{
		if(inputByte >> i & 1)
			parityCalc++;
	}

	if(parityCalc % 2 == 0)
		return 0 == parityBit;
	else
		return 1 == parityBit;
}

void TERMIO_PutChar(INT8 ch)
{
    // Poll for the last transmit to be complete
    do
    {
      // Nothing  
    } while (SCI0SR1_TC == 0);
    
    // write the data to the output shift register
    SCI0DRL = ch;
}

int convertInput(byte input) {
     int ret = 0;
     
     if(input & 0x10)
      ret = 2 - (~input + 1);
     else
      ret = input + 2;
     
     ret = (ret & 0x1f);
     return ret;
  
}

int POST() {
    while(firstInput) {}
    printf("%d\n", inputByte);
    
    * theServo.leds = (inputByte & 0x0f) << 4;
    
    return (inputByte & 0x0f) == 0x0f;
}

void main(void) {
  InitializeSerialPort();
  InitPorts();
  InitializeTimer(); 
  InitializePwm();
  
  //waitForButtonPushed();
  if(!POST())
    return;
  for(;;) {     

    if(setInput && checkParity()) {
      mov(&theServo, convertInput(inputByte));
      setInput = 0;
    }
  }
}



