#include "main.h"

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


void activate_door_open(unsigned int duration)
{
 unsigned int note_index = 0;
 const unsigned int door_open_frequency[] ={250, 110, 120, 400,750};
 const unsigned int door_open_note_duration[] = {300,  350,  400,  450,  500 };
   while(duration > 0)
  {
     
      run_Buzzer(door_open_frequency[note_index],door_open_note_duration[note_index]);
      if (note_index < 4)
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
void init_button()
{
 P1IE |= BIT3; // P1.3 interrupt enabled
 P1IES |= BIT3; // P1.3 Hi/lo edge
 P1IFG &= ~BIT3; // P1.3 IFG cleared
 
}

void play_knock_pattern()
{
  
  unsigned int index =0;
  while(index<5)
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


