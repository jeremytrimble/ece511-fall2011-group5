#include "main.h"

#define LED1 (BIT0) // Red LED
#define LED2 (BIT6) // Green LED

#define DELTA (unsigned long) 1200
#define KEY_LENGTH (unsigned int) 5
#define MIN_SILENCE (unsigned long) 18000

const unsigned long KEY[KEY_LENGTH] = { 0, 6000, 6000, 3000, 3000 };

#define KNOCK_HISTORY_LEN (8)
#define KNOCK_HISTORY_IDX_MASK (0x07)

unsigned int knockEventHistory[KNOCK_HISTORY_LEN] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char historyIdx = 0;

unsigned int failCounter = 0;

unsigned int index = 0;
bool wait_for_silence = FALSE;

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
  
  if (index == 0 && (!wait_for_silence || count >= MIN_SILENCE))
  {
    accept_knock();
  }
  else if (index > 0 && count >= (KEY[index] - DELTA)
           && count <= (KEY[index] + DELTA))
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
  P1OUT &= ~(LED1 | LED2);
  P1OUT |= LED2;  // temp
  
  if (index < KEY_LENGTH - 1)
  {
    index++;
  }
  else
  {
    P1OUT |= LED2;
    P1OUT |= LED1;  // temp
    
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
  
  wait_for_silence = FALSE;
}

void reject_knock()
{
  P1OUT &= ~(LED1 | LED2);
  
  // Failed!
  // TODO:  Do not immediately display rejection
  P1OUT |= LED1;
  
  failCounter++;
  
  bc_printf("Failed (%d times)!\n  INTRUDER ALERT!  Knock history: \n", failCounter);
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
  wait_for_silence = TRUE;
}  
