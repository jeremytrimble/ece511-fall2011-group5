#include "main.h"

/* No longer used as the alarm, but still used when playing back the
 * knock pattern.  For the new alarm code, see siren(). */
void activate_alarm(unsigned int duration)
{
  unsigned int note_index = 0;

  const unsigned int alarm_frequency [] ={100, 170, 200};
  const unsigned int alarm_note_duration[] = {500, 200,500};

 
 while(duration > 0 )
 {
   run_Buzzer(alarm_frequency[note_index], alarm_note_duration[note_index]);
   
   if (note_index < 2)
      {
        note_index++;
      }
      else
      {
        note_index = 0;
      }
      duration--; 
 }
}


/* Play a "happy" sound. */
void activate_door_open(void)
{
 unsigned int note_index = 0;
 const unsigned int door_open_frequency[] = {956, 630, 466};
 const unsigned int door_open_note_duration[] = {100, 150, 200};
 const unsigned short array_len = sizeof(door_open_frequency)/sizeof(door_open_frequency[1]);
 while (note_index < array_len)
  {
    run_Buzzer(door_open_frequency[note_index], door_open_note_duration[note_index]);
    note_index++;
  }

}
void init_button()
{
 P1IE |= BIT3; // P1.3 interrupt enabled
 P1IES |= BIT3; // P1.3 Hi/lo edge
 P1IFG &= ~BIT3; // P1.3 IFG cleared
 
}

void play_knock_pattern()
{
  
  unsigned int index =0;
  while(index<key_length_programmed)
    {
    delay_milliseconds((KEY[index]/10)+1);
    activate_alarm(1);
 index++;
    }
}

  
void init_buzzer(void)
{
 P2DIR |= BIT6; // P2.6 to output
 P2SEL &= ~BIT6; // Set P2SEL to 0
 P2OUT &=~BIT6;
}

void run_Buzzer(unsigned int half_clock_period, unsigned int note_duration)
{
  
 while(note_duration>0)
  {
   delay_microseconds(half_clock_period); // PWM Period
   //P2OUT |=BIT6;
   P2OUT ^= BIT6;
   note_duration--;
  }
  P2OUT &=~BIT6; 
  

}

void Stop_Buzzer (void)
{
P2OUT &=~BIT6;// turns off output to buzzer
}


/* This is used to sound the alarm after 3 incorrect knock sequences. */
void siren()
{
    unsigned int halfPeriod = 600;  /* [200, 1000] */
    unsigned int delay;
    int delta = 10;	/* Slews the frequency. */

    unsigned int halfCycles = 0;

    while ( halfCycles < 64 )	/* Controls duration of siren. */
    {
        delay = 5;
        while ( delay > 0 )
        {
            delay_microseconds(halfPeriod>>2); // PWM Period
            P2OUT ^= BIT6;
            delay--;
        }
        halfPeriod += delta;

        if ( halfPeriod == 1000 || halfPeriod == 200 )
            delta = -delta;

        if ( halfPeriod == 600 )
            halfCycles++;
    }
}
