#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "termio.h"
#include "stdio.h"
#include "stdlib.h"

unsigned int keyPressed, rowHigh;
unsigned int freqCountRow;
unsigned int freqCountColumn;

int rowFreq[4]={827, 749, 677, 612};
int columnFreq[4]={1435, 1299, 1174, 1063};
char decode[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

void	interrupt	11	OC3ISR(	){    //ISR for generating square waves
	TFLG1	|=	0x08;	
	TC3=TCNT+freqCountRow;
	//printf("Hello1\n");
}

void	interrupt	8	OC0ISR(	){     //ISR to measure time duration to stop the square wave
	TFLG1	|=	0x01;	
  TC0=TCNT+ freqCountColumn;
  //printf("-");
//printf("%d\n",freqCountColumn);
}

char keyDecode(int keyPressed, int rowHigh) {
 int row,column;
 switch(keyPressed) {
  case 16:
    column=0;
    break;
  case 32:
    column=1;
    break;
  case 64:
    column=2;
    break;
  case 128:
    column=3;
    break;
  default:
    column=-1;
    break;
 }
 switch(rowHigh) {
  case 1:
    row=0;
    break;
  case 2:
    row=1;
    break;
  case 4:
    row=2;
    break;
  case 8:
    row=3;
    break;
  default:
    row=-1;
    break;
 }
 if(row >=0 && row<=3 && column >=0 && column <= 3) {
   freqCountRow=rowFreq[row];
   freqCountColumn=columnFreq[column];
   TCTL2 = 0x41;
   TIOS = 0x09;
   TIE = 0x09;
   return decode[row][column];
 } else {
   return '';
 }
}

void main(void) {
  /* put your own code here */
  int i=0;
  int j=0;
  TERMIO_Init();
  DDRT |= 0x09;
  PTT = 0x00;
  DDRB |= 0x0f;
  PORTB |= 0x00;
  TSCR1 |= 0x80;
  TC3=10000;
  TC0=10000;
  EnableInterrupts;
  PORTB=0x02;
  TIE = 0x09;
  TIOS &= ~(0x09);
  while(1) {
    for(i=0;i<7000; i++);
    j++;
    //printf(". %d",PORTB);
    //detect if a key is pressed
    keyPressed = PORTB & 0xf0;
    rowHigh = PORTB & 0x0f;
    if(keyPressed!=0) {
      //printf("%d %d ",keyPressed, PORTB);
      printf("%c\n",keyDecode(keyPressed, rowHigh));
      j=0;
    } else {
      if(j>=4) {
       TCTL2 &= ~(0x41);
       TIOS &= ~(0x09);
       TIE &= ~(0x09);
      }
    }
    if(PORTB==0x08 || PORTB==0x00) {
      PORTB=0x01;
    } else {
      PORTB=PORTB<<1;
    }
  }
}
