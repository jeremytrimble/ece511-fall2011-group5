#include "main.h"

void activate_alarm(unsigned int duration)
{
  unsigned int note_index = 0;

const unsigned int alarm_frequency[] = {700, 250,333 };
const unsigned int alarm_delay[] =     {2,  2, 2}; 

 // init_buzzer();
 while(duration > 0 )
 {
     if (note_index < 3)
      {
        note_index++;
      }
      else
      {
        note_index = 0;
      }
      run_Buzzer(alarm_frequency[note_index], alarm_delay[note_index]);
 duration--; 
 }
}


void activate_door_open(unsigned int duration)
{
 unsigned int note_index = 0;
 const unsigned int door_open_frequency[] ={500, 600, 1000, 1200,1300};
 const unsigned int door_open_delay[] = {5,  5,  5,  5,  10 };
 
 //init_buzzer();
  while(duration > 0)
  {
     if (note_index < 5)
      {
        note_index++;
      }
      else
      {
        note_index = 0;
      }
      run_Buzzer(door_open_frequency[note_index],door_open_delay[note_index]);
  duration--;
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
  //const unsigned int KEY[5] = { 0, 6000, 6000, 3000, 3000 };
  unsigned int index =0;
  unsigned long timer =0;
  while(index++<5)
    {
    timer =15*KEY[index];
      while ((timer > 1))
      {timer--;}
    activate_alarm(50);
 
    }
}

  
void init_buzzer(void)
{
 P2DIR |= BIT6; // P2.6 to output
 P2SEL &= ~BIT6; // Set P2SEL to 0
 P2OUT |=BIT6;
 delay_milliseconds(250);// Turns the Buzzer On on Powerup
 P2OUT &=~BIT6;
}

void run_Buzzer(unsigned int half_clock_period, unsigned int delay)
{
  
 while(delay>0)
  {
   //delay_microseconds(half_clock_period); // PWM Period
   P2OUT |=BIT6;
   //P2OUT ^= BIT6;
   delay--;
  }
  P2OUT &=~BIT6; 
  

}

void Stop_Buzzer (void)
{
P2OUT &=~BIT6;// turns off output to buzzer
}


