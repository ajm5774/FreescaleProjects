void initButton()   
{  
  //turn on clock to Port A  
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  
  
  // configure pin to be GPIO  
  PORTA_PCR16 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;  
  
  // configure PTA16 to be input  
  GPIOA_PDDR &= (0<<16);    
} 

void waitForButtonPushed () {               
    while(!(GPIOA_PDIR & (1<<16))) 
    {
       return;  
    } 
}  