#include "main.h"

#define BUTTON (BIT3)

#include "timing.h"

unsigned long timer = 0;

extern void init_detection()
{
    // Switch is active-low, so trigger on press, not release.
    P1IES |= BUTTON;
}

extern void run_detection()
{
    while (1)
    {
        P1IFG &= ~BUTTON;
        while ( !(P1IFG & BUTTON) )
        {
            delay_milliseconds(1);
            timer++;
        }

        report_knock(timer);
        timer = 0;

        // wait for release (switch is active low)
        while ( !(P1IN & BUTTON) );
    }
}
