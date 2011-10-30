#ifndef __BACKCHANNEL_COMMS_H_
#define __BACKCHANNEL_COMMS_H_

#include <msp430G2231.h>

/*
 *  WARNING:
 *   It seems that if you start sending out data on the backchannel uart as
 *   soon as the processor starts, it interferes with the proper setup of the
 *   Launchpad as a USB device, so avoid sending data on the backchannel UART
 *   immediately on startup.
 *
 *   If you really need to do this, then you can disconnect the "TXD" jumper
 *   when plugging your LaunchPad into your PC's USB port.  This seems to keep
 *   things from going haywire.
 */

/* Sets up the "backchannel UART" for sending data from the LaunchPad to the PC
 * through a "virtual COM port" over USB.
 *
 * The "virtual COM port" shows up as
 *   "MSP430 Application UART"
 * in Windows Device Manager.
 *
 * You can view to the output of the MSP430 backchannel uart through
 * HyperTerminal with the following settings:
 *  Bits per second:    9600
 *  Data bits:          8
 *  Parity:             None
 *  Stop bits:          1
 *  Flow Control:       None
 */
void init_backchannel(void);  

/* Sends a single character over the backchannel UART. */
void bc_tx_byte(const unsigned char);

/* A printf()-like function that sends output over the backchannel UART.
 *
 *  Works like your typical printf(), except only the following format
 *  directives are supported:
 *      %d -> signed 8-bit or 16-bit integer    (char, short, int)
 *      %u -> unsigned 8-bit or 16-bit integer  (char, short, int)
 *      %c -> a single character (printed as ASCII)
 *      %x -> prints a signed or unsigned 8 or 16-bit number as hexadecimal
 *
 *  Also, width specifiers (e.g. %5d or %06d) and other such fancy directives
 *  aren't allowed either.  (Sorry.)
 *
 *  So, for instance, to print out an integer value called "myValue":
 *  {
 *      int myValue;
 *      // ...
 *      printf("This is myValue: %d\n", myValue);
 *  }
 *
 *  Note:
 *   Any time the '\n' (newline) character is seen in the format string,
 *   a '\r' (linefeed) character is also sent, so that a new line of text
 *   begins at the leftmost column.
 *
 *  I'm not sure if it's a good idea to use this function inside an ISR.
 *
 */
void bc_printf(const char * fmt, ...);

/*
 * Returns a value in range [0, 255] on success.
 * Returns -1 if no data currently available.
 */
int bc_rx_byte(void);

#endif

