#include "main.h"
unsigned int KEY[KEY_LENGTH] = { 0, 6000,  6000, 3000, 3000, 0, 0, 0, 0 };
unsigned int index = 0;

void init_programming_mode( )
{
  programming_mode =0;
 }
void start_programming(unsigned long timer)
{
  
  KEY[index]=timer;
  
  if  (index < KEY_LENGTH)
      {
        index++;
      }
      else
      {
        index = 0;
      } 
  
  
  
}
