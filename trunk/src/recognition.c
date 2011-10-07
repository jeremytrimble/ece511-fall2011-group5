#include "main.h"

#include "backchannel.h"

#define LED1 (BIT0) // Red LED
#define LED2 (BIT6) // Green LED

#define DELTA 100

const unsigned long KEY[] = { 0, 1000, 1000, 500, 500 };

#define KNOCK_HISTORY_LEN (8)
#define KNOCK_HISTORY_IDX_MASK (0x07)
unsigned int knockEventHistory[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char historyIdx = 0;

unsigned int failCounter = 0;

unsigned int index = 0;

void accept_knock();
void reject_knock();

extern void init_recognition() {
  P1DIR |= (LED1 | LED2);
  P1OUT &= ~(LED1 | LED2);
}

extern void report_knock(unsigned long count)
{
    knockEventHistory[historyIdx] = (unsigned int) count;
    historyIdx = (historyIdx + 1) & KNOCK_HISTORY_IDX_MASK;

  if (index == 0 || (count >= (KEY[index] - DELTA)
                     && count <= (KEY[index] + DELTA) ))
  {
    accept_knock();
  }
  else
  {
    reject_knock();
  }
}

void accept_knock()
{
  P1OUT &= ~LED1;
  P1OUT |= LED2;
  if (index < 4)
  {
    index++;
  }
  else
  {
    P1OUT |= LED1;

    // Succeeded!
    bc_printf("Success!\n  Knock history: ");
    {
        int i;
        for (i = 0; i < KNOCK_HISTORY_LEN; i++)
        {
            bc_printf("%u ",
                knockEventHistory[ (i + historyIdx) & KNOCK_HISTORY_IDX_MASK]);
        }
    }
    bc_printf("\n");

    index = 0;
    failCounter = 0;
  }
}

void reject_knock()
{
  P1OUT &= ~LED2;
  P1OUT |= LED1;

  // Failed!
  failCounter++;

  bc_printf("Failed (%d times)!\n  INTRUDER ALERT!\n", failCounter);

  index = 0;
}  
