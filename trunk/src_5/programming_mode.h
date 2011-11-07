#ifndef __PROGRAMMING_MODE_H
#define __PROGRAMMING_MODE_H

#define KEY_LENGTH (unsigned int) 10
extern unsigned int KEY[KEY_LENGTH];
extern void start_programming(unsigned long timer);
extern void init_programming_mode( );
extern unsigned int key_length_programmed;

#endif