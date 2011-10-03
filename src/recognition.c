#include "main.h"

#define LED1 (BIT0)
#define LED2 (BIT6)

#define DELTA 100
#define MS_FACTOR 32

const unsigned long KEY[] = { 0, 1000, 1000, 500, 500 };

unsigned int index = 0;

void accept_knock();
void reject_knock();

extern void init_recognition() {
  P1DIR = (LED1 | LED2);
  P1OUT &= ~(LED1 | LED2);
}

extern void report_knock(unsigned long count)
{
  if (index == 0 || (count >= (KEY[index] - DELTA) * MS_FACTOR
                     && count <= (KEY[index] + DELTA) * MS_FACTOR))
  {
    accept_knock();
    index++;
  }
  else
  {
    reject_knock();
    index = 0;
  }
}

void accept_knock()
{
  P1OUT &= ~LED1;
  P1OUT |= LED2;
}

void reject_knock()
{
  P1OUT &= ~LED2;
  P1OUT |= LED1;
}  
