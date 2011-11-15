#ifndef __BUZZER_H
#define __BUZZER_H
#include <msp430G2231.h>
//unsigned int Duty_Cycle;
extern void init_buzzer(void);
extern void run_Buzzer(unsigned int half_clock_period, unsigned int delay);
extern void delay_ms(unsigned int delay);
extern void Stop_Buzzer();
extern void activate_alarm(unsigned int duration);
extern void activate_door_open(unsigned int duration);
extern void init_button();
extern void play_knock_pattern( );
#endif
