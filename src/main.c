#include "main.h"

void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  
  init_detection();
  init_recognition();
  
  __enable_interrupt();
  
  run_detection();
}
