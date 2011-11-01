#include "main.h"


void main(void)
{
 init_clock();
 init_backchannel();
 init_detection();
 init_recognition();
 
 init_button();
 

 __enable_interrupt();
 init_buzzer();
 check_solonoid();
 check_led();
 check_buzzer();
 run_detection();
}
