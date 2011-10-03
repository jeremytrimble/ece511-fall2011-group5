#include "main.h"

#define BUTTON (BIT3)

unsigned long timer = 0;

extern void init_detection()
{
  P1IE = BUTTON;
  P1IFG = 0;
}

extern void run_detection()
{
  while(1)
  {
    timer++;
  }
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{
  P1IFG &= ~BUTTON;
  
  report_knock(timer);
  timer = 0;
}
