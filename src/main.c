#include "recognition.h"

#define LED1 (BIT0)
#define LED2 (BIT6)

const unsigned long SEQUENCE[] = { 1000, 1000, 500, 500 };

void main(void) {
  WDTCTL = WDTPW | WDTHOLD;
}
