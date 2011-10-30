#include "main.h"


void main(void)
{
 init_clock();
 init_backchannel();
 init_detection();
 init_recognition();
 init_buzzer();
 init_button();
 __enable_interrupt();
 run_detection();
}
