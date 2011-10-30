#ifndef __RECOGNITION_H
#define __RECOGNITION_H
#define LED1 (BIT0) // Red LED
#define LED2 (BIT6) // Green LED
#define SOL_CTRL (BIT7) // Solenoid controller

#define DELTA (unsigned long) 700   /* TODO: experiment more with this. */
#define KEY_LENGTH (unsigned int) 5
#define RESET_TIME (unsigned int) 375

#define KNOCK_HISTORY_LEN (8)
#define KNOCK_HISTORY_IDX_MASK (0x07)
extern void init_recognition();

extern void report_knock(unsigned long count);

extern const unsigned int KEY[KEY_LENGTH];


#endif

