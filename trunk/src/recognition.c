#include "main.h"

#define LED1 (BIT0) // Red LED
#define LED2 (BIT6) // Green LED
#define SOL_CTRL (BIT7) // Solenoid controller

#define DELTA (unsigned long) 450   /* TODO: experiment more with this. */
#define KEY_LENGTH (unsigned int) 5
#define RESET_TIME (unsigned int) 375

#define KNOCK_HISTORY_LEN (8)
#define KNOCK_HISTORY_IDX_MASK (0x07)

const unsigned int KEY[KEY_LENGTH] = { 0, 6000, 6000, 3000, 3000 };

unsigned int knockEventHistory[KNOCK_HISTORY_LEN] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char historyIdx = 0;

unsigned int failCounter = 0;

unsigned int index = 0;

bool wait_for_reset = FALSE;

unsigned int timer_count = 0;

/* Fixed-point scale factor in 4.4 format. 
 * 4 binary digits to the left of the "virtual decimal point".
 * 4 binary digits to the right of the "virtual decimal point". 
 */
unsigned char scale_factor = 0;

void reset();
void accept_knock();
void reject_knock();

extern void init_recognition()
{
  P1DIR |= (LED1 | LED2 | SOL_CTRL);
  P1OUT &= ~(LED1 | LED2 | SOL_CTRL);
  
  WDTCTL = WDT_MDLY_8;
  IE1 |= WDTIE;
}

extern void report_knock(unsigned long count)
{
  timer_count = 0;
  TACTL |= TACLR;
  P1OUT &= ~(LED1 | LED2 | SOL_CTRL);
  
  knockEventHistory[historyIdx] = (unsigned int) count;
  historyIdx = (historyIdx + 1) & KNOCK_HISTORY_IDX_MASK;
  
  if (index == 0 && (!wait_for_reset))
  {
    accept_knock();
  }
  else if (index > 0)
  {
      if ( 1 == index )
      {
        // Compute scale factor here
        // scale_factor = KEY[1] / count
        unsigned long dividend = (unsigned long)count << 4;
        unsigned long divisor  = KEY[1];
        scale_factor = 0;
        while (dividend >= divisor)     // Poor-man's division.
        {
            scale_factor++;
            dividend -= divisor;
        }
      }

      // Apply scale factor.
      unsigned long scaled_key = ( scale_factor * (unsigned long)KEY[index] ) >> 4;

      if ( 
          ( /* If scaled_key-DELTA would be < 0, don't do comparison */
           ( scaled_key < DELTA )
           ||
           ( (scaled_key >= DELTA) && ( count >= ( scaled_key - DELTA ) ) )
          )           
          && /* If scaled_key + delta would cause overflow, don't do comparison. */
          ( ((unsigned int)1<<15) - scaled_key < DELTA )  
          ||
          ( ((unsigned int)1<<15) - scaled_key >= DELTA &&  count <= ( scaled_key + DELTA ) )
         )
      {
        accept_knock();
      }
      else
      {
          reject_knock();
      }
  }
  else
  {
    reject_knock();
  }
}

void accept_knock()
{
  if (index < KEY_LENGTH - 1)
  {
    index++;
  }
  else
  {
    P1OUT |= LED2;
    P1OUT |= SOL_CTRL;
    
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
    bc_printf("\n Scale factor: (%u >> 4)\n", scale_factor);
    
    index = 0;
    failCounter = 0;
  }
}

void reject_knock()
{
  // Failed!
  failCounter++;
  
  bc_printf("Failed (%u times)!\n  INTRUDER ALERT!  Knock history: \n", failCounter);
  {
    int i;
    for (i = 0; i < KNOCK_HISTORY_LEN; i++)
    {
      bc_printf("%u ",
                knockEventHistory[ (i + historyIdx) & KNOCK_HISTORY_IDX_MASK]);
    }
  }
  bc_printf("\n Scale factor: (%u >> 4)\n", scale_factor);
  
  index = 0;
  wait_for_reset = TRUE;
}  

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer()
{
  if (timer_count < RESET_TIME)
  {
    timer_count++;
  }
  else
  {
    if (wait_for_reset)
    {
      P1OUT |= LED1;
    }
    else
    {
      P1OUT &= ~LED1;
    }
    
    timer_count = 0;
    index = 0;
    wait_for_reset = FALSE;
  }
  
}
