#include "main.h"
unsigned int KEY[KEY_LENGTH] = { 0, 6000, 6000, 3000, 3000, 0, 0, 0, 0, 0 };
unsigned int key_length_programmed = 5;
unsigned int programming_index =0;
void init_programming_mode( )
{
  programming_mode =0;
  programming_index=0;
}

void start_programming(unsigned long timer)
{
  if (programming_index == 0)// if it is the first knock
  {
  KEY[programming_index]= 0;
  }
  else
  {
  KEY[programming_index]=timer;
  }
  
 if  (programming_index < KEY_LENGTH)
 {
    programming_index++;
 }
 else
 {
  programming_index = 0;
  
 } 
  
  key_length_programmed = programming_index;//arrays elements are numbered starting from zero
  
}


  
