#include "main.h"

#define LED1 (BIT0)
#define LED2 (BIT6)

const unsigned long SEQUENCE[] = { 1000, 1000, 500, 500 };

void report_knock() {
  P1OUT |= LED1;
  for (int i=0; i < 2 * DELTA * MS_FACTOR; i++);
  P1OUT ^= LED1;
}
