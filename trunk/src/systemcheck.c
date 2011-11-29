#include "main.h"

void check_buzzer(void)
{
    bc_printf("Checking The Buzzer... ");

    /* Play the "cavalry charge" song.
     * Frequencies and delays computed in Matlab/octave by:
     *
     * f = [392.00, 523.25, 659.26, 783.99, 659.26, 783.99];
     * t = [1 1 1 2 1 2]*100;
     * half_periods = round((1./f)*1e6/2)
     * durations = round(half_periods(1)./half_periods .* t)
     *
     */
    run_Buzzer(1276, 100);
    run_Buzzer(956,  133);
    run_Buzzer(758,  168);
    run_Buzzer(630,  400);  /* Was 638, but sounded a little flat. */
    delay_microseconds(100);
    run_Buzzer(758,  168);
    run_Buzzer(630,  600);

    bc_printf("CHARGE!\n");
}

void check_solonoid (void)
{
  bc_printf("Checking Solenoid Control.....\n ");
  P1OUT |= SOL_CTRL;
  delay_milliseconds(500);
  P1OUT &=~ SOL_CTRL;
  delay_milliseconds(500);
  P1OUT |= SOL_CTRL; 
  delay_milliseconds(500);
  P1OUT &=~ SOL_CTRL;
  
}
void check_microphone(void)
{
  
  
}

void check_led(void)
{
  bc_printf("Checking LEDs ......\n  ");
  P1OUT |= LED2;
  P1OUT &=~ LED1;
  delay_milliseconds(500);
  P1OUT &=~ LED2;
  P1OUT |= LED1;
  delay_milliseconds(500);
  P1OUT |= LED2;
  P1OUT &=~ LED1;
  delay_milliseconds(500);
  P1OUT &=~ LED2;
  P1OUT |= LED1;
  delay_milliseconds(500);
  P1OUT |= LED2;
  P1OUT &=~ LED1;
  delay_milliseconds(500);
   P1OUT |= LED2;
  P1OUT |= LED1;
}
