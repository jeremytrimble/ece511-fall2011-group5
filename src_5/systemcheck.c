#include "main.h"

void check_buzzer(void)
{
 bc_printf("Checking The Buzzer .......!\n  ");
   activate_door_open(8);
}

void check_solonoid (void)
{
  bc_printf("Checking Solonoid Control.....\n ");
  P1OUT |= SOL_CTRL;
  delay_milliseconds(1500);
  P1OUT &=~ SOL_CTRL;
  delay_milliseconds(1500);
  P1OUT |= SOL_CTRL; 
  delay_milliseconds(1500);
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