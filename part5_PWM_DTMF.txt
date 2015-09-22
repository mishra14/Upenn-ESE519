#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "termio.h"
#include "stdio.h"
#include "stdlib.h"

unsigned int keyPressed, rowHigh;
unsigned int freqCountRow;
unsigned int freqCountColumn;

int rowFreqHigh[4]={0x0B, 0x0a, 0x09, 0x08};
int rowFreqLow[4]={0x2c, 0x14, 0x24, 0x48};

int rowFreqHighDTY[4]={0x05, 0x05, 0x04, 0x04};
int rowFreqLowDTY[4]={0x96, 0x0a, 0x92, 0x24};

int columnFreqHigh[4]={0x06, 0x05, 0x05, 0x04};
int columnFreqLow[4]={0x76, 0xd8, 0x4a,  0xc9};

int columnFreqHighDTY[4]={0x03, 0x02, 0x02, 0x02};
int columnFreqLowDTY[4]={0x3b, 0xec, 0xa5, 0x64};
char decode[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};


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
    PWMPER4=rowFreqHigh[row];
    PWMPER5=rowFreqLow[row];
    PWMDTY4=rowFreqHighDTY[row];
    PWMDTY5=rowFreqLowDTY[row]; 
    
    PWMPER2=columnFreqHigh[column];
    PWMPER3=columnFreqLow[column];
    PWMDTY2=columnFreqHighDTY[column];
    PWMDTY3=columnFreqLowDTY[column];
    PWME = 0x3c;
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
  
  PWMCLK = 0x00;
  PWMPOL = 0x3c;
  PWMCTL = 0x60;
  PWMPRCLK = 0x00;
  while(1) {
       for(i=0;i<7000; i++);
    j++;
    printf(". %d%d",PWMPER2, PWMPER3);
    //detect if a key is pressed
    keyPressed = PORTB & 0xf0;
    rowHigh = PORTB & 0x0f;
    if(keyPressed!=0) {
      //printf("%d %d ",keyPressed, PORTB);
      printf("%c\n",keyDecode(keyPressed, rowHigh));
      j=0;
    } else {
      if(j>=4) {
         PWME = 0x00;
      }
    }
    if((PORTB & 0x0f)==0x08 || (PORTB & 0x0f)==0x00) {
      PORTB=0x01;
    } else {
      PORTB=PORTB<<1;
    }
  }
}
