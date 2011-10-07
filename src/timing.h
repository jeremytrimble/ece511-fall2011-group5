#ifndef _TIMING_H__
#define _TIMING_H__

#include <msp430G2231.h>

/* Note:  If changing the clock frequency established by InitializeClocks(),
 * comment out the line below.  Any code which explicitly relies on the clock
 * frequency being 1MHz should issue an "#error" if MCLK_IS_1MHZ is not
 * #defined. */
#define MCLK_IS_1MHZ

void init_clock(void);

/* Delays for the specified number of microseconds.
 * Returns after at least duration_usec microseconds have elapsed. */
void delay_microseconds(unsigned int duration_usec);

/* Delays for the specified number of millliseconds.
 * Returns after at least duration_msec milliseconds have elapsed. */
void delay_milliseconds(unsigned int duration_msec);

#endif
