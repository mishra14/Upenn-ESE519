#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "termio.h"
#include "stdio.h"


void main(void) {
  TERMIO_Init();
	//EnableInterrupts;
	DDRB = 0xf0;
  PORTB = 0xf0;
  ATDCTL2 = 0xC0;
  ATDCTL3 = 0x18;
  ATDCTL4 = 0x85; 
  ATDCTL5 = 0x20;
  while(1) {
     if(ATDDR0H<=49) {
       printf("%d\n", ATDDR0H);
      PORTB=0x00;
     } else if(ATDDR0H>50 && ATDDR0H<=100) {
       printf("%d\n", ATDDR0H);
      PORTB=~(0x10);
     }
     else if(ATDDR0H>100 && ATDDR0H<=150) {
       printf("%d\n", ATDDR0H);
      PORTB=~(0x20);
     }
     else if(ATDDR0H>150 && ATDDR0H<=200) {
       printf("%d\n", ATDDR0H);
      PORTB=~(0x40);
     }
      else {
       printf("%d\n", ATDDR0H);
       PORTB=~(0x80);
     }
     
  }
}
