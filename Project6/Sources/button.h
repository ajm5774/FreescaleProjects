#ifndef ENGINE_H
#define ENGINE_H

#define BUTTONLENGTH 5
#define ON 1
#define OFF 0  
  
// Button's Defined State   
// 0 means button not pressed  
// 1 means button pressed  
short buttonState = 0;    
  
void initButton();

void waitForButtonPushed();