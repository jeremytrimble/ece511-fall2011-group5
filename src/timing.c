#include "timing.h"

#ifdef MCLK_IS_1MHZ

/* Configures the Basic Clock Module
 * The DCO (Digitally-Controlled Oscillator) is configured to generate a 1MHz
 * clock frequency.
 *
 * This signal drives _both_ MCLK (Master Clock, which drives the CPU) and
 * SMCLK (Subsystem Master Clock, which can be used by peripherals like
 * TIMER_A).
 *
 * For details, see:
 *  - doc/datasheets/msp430/slau44h.pdf, page 275
 *  - doc/datasheets/msp430/slau/msp430g2231.pdf, page 23
 *
 * Note:  If you intend to change the clock frequency to something other than
 * 1MHz, other code may be affected and need updated.
 */
void init_clock(void)
{

    BCSCTL1 = CALBC1_1MHZ;    // Set RSELx (range selection) on DC Generator.
    DCOCTL = CALDCO_1MHZ;     // Set modulator selection, to generate 1MHz.
    BCSCTL2 &= ~(DIVS_3);     // SMCLK = DCO = 1MHz  

    /* NOTE: The line below should match the line used in init_backchannel() */
    TACTL = TASSEL_2 /* Use SMCLK */
            | ID_0  ;

}

void delay_microseconds(unsigned int duration_usec)
{
    istate_t istate = __get_interrupt_state();
    __disable_interrupt();

    // Stop the timer.
    TACTL &= ~0x0030;

    // Clear the timer.
    TACTL |= TACLR;

    /* Since SMCLK clock is 1 MHz, the clock that drives TimerA is SMCLK/8 or
     * 125 kHz.  Thus, each tick of TAR is 8 microseconds. */
    TACCR0 = duration_usec;

    TACTL &= ~(TAIE | TAIFG);           // Make sure interrupts are off and interrupt flag is clear.

    // Start the timer (count up to value of TACCR0).
    TACTL |= 0x0010;
    
    while ( ! (TACTL & TAIFG) );  // Wait until interrupt flag is set.

    __set_interrupt_state(istate);
}

void delay_milliseconds(unsigned int duration_milliseconds)
{
    /* Internally call delayMicroseconds */
    while ( duration_milliseconds > 65 )
    {
        delay_microseconds( (unsigned)65 * 1000 );
        duration_milliseconds -= 65;
    }

    delay_microseconds( duration_milliseconds * 1000 );
}

#else
#error "MCLK is not 1 MHz.  Need to update assumptions in this code."
#endif
