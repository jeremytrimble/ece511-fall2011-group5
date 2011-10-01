#include "main.h"

#define LED1 (BIT0)
#define LED2 (BIT6)

void main(void) {
  WDTCTL = WDTPW | WDTHOLD;
  
  __low_power_mode_4();
}
