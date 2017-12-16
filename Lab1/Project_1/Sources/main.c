#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "stdio.h"
#include "termio.h"

unsigned int highEdge, diff;
unsigned long pulseWidth, overflow;
char ch;
void interrupt 10 IC2ISR(void) {
  //printf(". \n");
  TFLG1=0x04;
  if(PTT&(0x04)) {
    PORTB|=0x40;
    highEdge = TCNT;
    overflow=0;
   } else {
    PORTB&=~(0x40);
    diff=TCNT-highEdge;
    if(TCNT<highEdge) {
     overflow-=1; 
    }
    pulseWidth=overflow *65536u + (long)diff;
    //printf("%lu\n",pulseWidth);
    pulseWidth=pulseWidth/2000;
    if(pulseWidth>30 && pulseWidth<=200) {
     ch='.'; 
    } else if(pulseWidth>200 && pulseWidth<=400) {
     ch='-'; 
    }
    else if(pulseWidth>400) {
     ch=' '; 
    }
    printf("%c",ch);
    
   }  
}
void interrupt 16 TOFISR(void) {
 TFLG2|=0x80;
 overflow++;
}
void main(void) {
   TERMIO_Init();
   DDRB|=0x70;
   PORTB|=0x50;
   DDRT&=~(0x04);
   TCTL4|=0x30;
   TIE|=0x04;
   TSCR1|=0x80;
   TSCR2|=0x80;
   overflow=0;
   EnableInterrupts;
  while(1) {
    
  }
}
