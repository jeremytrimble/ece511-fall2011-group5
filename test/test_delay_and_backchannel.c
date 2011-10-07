#include <msp430G2231.h>

#include "timing.h"
#include "backchannel.h"

#define RED_LED (BIT0)     // Red LED
#define GRN_LED (BIT6)     // Green LED

int main( void )
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer to prevent time out reset
    P1OUT = 0;                  // Set P1 outputs to LOW.
    P1DIR |= RED_LED|GRN_LED;   // Set LED Pins to OUTPUT

    P1OUT |= RED_LED;   // Turn on Green LED.

    init_clock();
    init_backchannel();


    // Wait 30 seconds for USB interface to come up.
    //delay_milliseconds( 30000 );

    while ( P1IN & 0x08 );

    bc_printf("\nthis is a test: %u %d %c %x %d yay\n", (unsigned int) 34567, -42, 'j', 0xdead, 23);
    bc_printf("\nthis is also a test: %x %d yay\n", 'A', 235);

    {
        int i=5;
        while ( i )
        {
            bc_printf("countdown: %d\n", i--);
            delay_milliseconds(1000);
        }
    }

    {
        signed char ctr = 120;
        while (1)
        {
            delay_milliseconds(10);     // debounce
            while ( P1IN & 0x08 );      // wait for press
            //delay_milliseconds(10);     // debounce
            //while ( !(P1IN & 0x08) );   // wait for release

            bc_printf("ctr is: %d\n", ctr++);

            P1OUT ^= GRN_LED | RED_LED;

            if ( !ctr )
            {
                int i=5;
                while ( i )
                {
                    bc_printf("countdown: %d\n", i--);
                    delay_milliseconds(1000);
                }
            }

            {
                int chr;
                int didprint = 0;
                while ( (chr = bc_rx_byte()) > 0 )
                {
                    if ( !didprint )
                    {
                        bc_printf("you typed: ");
                        didprint = 1;
                    }

                    bc_printf("%c ", (unsigned char)chr);
                }

                if ( didprint )
                    bc_printf("\n");
            }
        }
    }

    P1OUT &= ~GRN_LED;  // Turn off Green LED.
    P1OUT |= RED_LED;   // Turn on Red LED.
    return 0;
}
