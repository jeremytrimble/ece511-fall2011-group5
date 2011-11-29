#ifndef __BUZZER_H
#define __BUZZER_H
#include <msp430G2231.h>
//unsigned int Duty_Cycle;
void init_buzzer(void);
void run_Buzzer(unsigned int half_clock_period, unsigned int delay);
void delay_ms(unsigned int delay);
void Stop_Buzzer();
void activate_alarm(unsigned int duration);
void activate_door_open(void);
void init_button();
void play_knock_pattern( );
void siren();
#endif
