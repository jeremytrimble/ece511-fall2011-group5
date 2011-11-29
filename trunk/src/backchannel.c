#include "main.h"     /* MCLK_IS_1MHZ */

#include <stdarg.h>

#ifdef MCLK_IS_1MHZ
//   Conditions for 9600 Baud SW UART, SMCLK = 1MHz
#define     Bitime                8*13-1 //13      // 13/125k = duration of 1 bit at 9600 baud
#define     TXD                   BIT1    // TXD on P1.1
#define     RXD                   BIT2    // RXD on P1.2
#else
#error "MCLK is not 1 MHz.  Need to update assumptions in this code."
#endif

/* These variables allow bc_tx_byte()
 * to communicate with the Timer_A() ISR. */
static volatile unsigned int TXByte;
static volatile unsigned char BitCnt;
unsigned int programming_mode; 
void init_backchannel(void)
{
    /* TX SETUP */
    CCTL0 = OUT;                              // TXD Idle as Mark

    // This line should be the same as the one used in init_clock()
    TACTL = TASSEL_2 /* Use SMCLK */
            | ID_0  ;

    /* Switch P1.1 to TA0.0 This enables the "secondary function"
    * (Timer A output) for this pin. */
    P1SEL |= TXD;

    P1DIR |= TXD;                             // Set TX pin as output.


    /* RX SETUP */
    P1DIR &= ~RXD;      // Set RX pin as input.

    // Set RX pin as plain old GPIO.
    P1SEL  &= ~RXD;
    //P1SEL2 &= ~RXD;

    // Set RX pin to trigger interrupt on falling edge.
    // (The first falling edge is the beginning of the start bit).
    P1IES |= RXD;   // High-to-low

    P1IFG &= ~RXD;  // Clear interrupt flag.

    P1IE |= RXD;    // Enable interrupts for RX pin.
    __enable_interrupt();                   // Set GIE


    // Note: BIT5 can be used to mark the exact sample instants For debugging.
    // (You'll need to hook up an oscilloscope to RXD and P1.5, and trigger on
    // the falling edge of RXD).
    // To debug, uncomment the P1DIR line below.
    //P1DIR |= BIT5;
    P1OUT &= ~BIT5;     // (debug)
}

// Function Transmits Character from TXByte
void bc_tx_byte(const unsigned char byte)
{
    TXByte = byte;

    // Stop the timer.
    TACTL &= ~0x0030;

    BitCnt = 0xA;                           // Load Bit counter, 8data + ST/SP

    // Clear the timer.
    TACTL |= TACLR;

    // Set end of bit time.
    TACCR0 = Bitime;

    TXByte |= 0x100;                        // Add mark stop bit to TXByte
    TXByte = TXByte << 1;                   // Add space start bit
    CCTL0 =  CCIS0 + OUTMOD0 + CCIE;        // TXD = mark = idle
 
    TACTL &= ~(TAIE | TAIFG);               // Make sure interrupts are off and interrupt flag is clear.

    __enable_interrupt();                   // Set GIE
 
    // Start the timer (count up to value of TACCR0).
    TACTL |= 0x0010;

    while ( TACCTL0 & CCIE );           // Wait for TX completion
}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
    //if (CCTL0 & CCIFG)                      // TX on CCI0B?
    {
      if ( BitCnt == 0 )
      {        
        CCTL0 &= ~ CCIE ;                   // All bits TXed, disable interrupt
      }  
     
      else
      {
        CCTL0 |=  OUTMOD2;                  // TX Space
        if (TXByte & 0x01)
          CCTL0 &= ~ OUTMOD2;                 // TX Mark
        TXByte = TXByte >> 1;
        BitCnt --;
      }
    }
}

#define FIXUP_DIGIT(i)      \
while ( digits[(i)] >= 10 ) \
{                           \
    digits[i] -= 10;        \
    digits[i+1]++;          \
}

static void send_unsigned_number(unsigned int num)
{
    // There can be at most 5 decimal digits in a 16-bit number.
    unsigned int digits[5];
    digits[4] = digits[3] = digits[2] = digits[1] = digits[0] = 0;

    unsigned int bitPosition = 0x1;
    while (bitPosition != 0 )
    {
        digits[0] += num & bitPosition;
        FIXUP_DIGIT(0);
        FIXUP_DIGIT(1);
        FIXUP_DIGIT(2);
        FIXUP_DIGIT(3);

        bitPosition = bitPosition << 1;
    }

    int i;
    int initial = 1;
    for (i = 4; i >= 0; i--)
    {
        if ( !initial || digits[i] > 0 || 0 == i )
        {
            bc_tx_byte( digits[i] + '0' );
            initial = 0;
        }
    }
}

#undef FIXUP_DIGIT

/* A basic printf()-like function. */
void bc_printf(const char * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

    char prevChar = ' ';
    char curChar;
 
    while ( 0 != *fmt )
    {
        curChar = *fmt;

        if ( '%' != prevChar )
        {
            if ( '%' != curChar )
            {
                // Plain character.  Print it.
                bc_tx_byte(curChar);

                // Hyperterminal needs \n\r, not just \n.
                if ( '\n' == curChar )
                    bc_tx_byte('\r');
            }
            else
            {
                /* Don't do anything, this might
                 * be the beginning of a control sequence. */
            }
        }
        else
        {
            // Might be a control sequence.
            if ( 'u' == curChar )
            {
                // Send an unsigned number
                unsigned int val = va_arg(arglist, unsigned int);

                send_unsigned_number( val );
            }
            else if ( 'd' == curChar )
            {
                // Send a signed number.
                int val = va_arg(arglist, int);

                if ( val < 0 )
                    bc_tx_byte('-');

                send_unsigned_number( val < 0 ? -val : val );
            }
            else if ( 'c' == curChar )
            {
                // Send a single character.
                unsigned char val = va_arg(arglist, unsigned char);
                bc_tx_byte(val);
            }
            else if ( 'x' == curChar )
            {
                // Send as a hex string.
                unsigned int val = va_arg(arglist, unsigned int);
                int tmp;

                bc_tx_byte('0');
                bc_tx_byte('x');

                tmp = ((val & 0xf000) >> 12) & 0xf;
                bc_tx_byte( tmp + (tmp >= 10 ? 'A'-10 : '0') );
                val = val << 4;

                tmp = ((val & 0xf000) >> 12) & 0xf;
                bc_tx_byte( tmp + (tmp >= 10 ? 'A'-10 : '0') );
                val = val << 4;

                tmp = ((val & 0xf000) >> 12) & 0xf;
                bc_tx_byte( tmp + (tmp >= 10 ? 'A'-10 : '0') );
                val = val << 4;

                tmp = ((val & 0xf000) >> 12) & 0xf;
                bc_tx_byte( tmp + (tmp >= 10 ? 'A'-10 : '0') );
                val = val << 4;
            }
            else if ( '%' == curChar )
            {
                // Send a single '%'
                bc_tx_byte('%');

                // Trick:  Don't confuse next iteration.
                curChar = ' ';
            }
            else
            {
                // Ok it wasn't a control sequence.  Just print it I guess...
                bc_tx_byte('%');
                bc_tx_byte(curChar);

                // Hyperterminal needs \n\r, not just \n.
                if ( '\n' == curChar )
                    bc_tx_byte('\r');
            }
        }

        prevChar = curChar;
        fmt++;
    }

    va_end(arglist);
}

#define RX_BUFFER_DEPTH_BYTES (16)  // NOTE: depends on mask
#define RX_BUFFER_DEPTH_MASK (0x0F) // NOTE: depends on depth

static volatile unsigned char rxbuf_next_read_idx = 0;
static volatile unsigned char rxbuf_next_write_idx = 0;
static volatile unsigned char rxbuf[RX_BUFFER_DEPTH_BYTES];

int bc_rx_byte(void)
{
  unsigned char rxbuf_next_read_idx_var= 0;
  unsigned char rxbuf_next_write_idx_var =0;
  
  int rv;

    // This needs to be atomic, so momentarily disable RX interrupt.
    P1IE &= ~RXD;   // Disable interrupts for RX pin.

    // BEGIN ATOMIC:
    rxbuf_next_read_idx_var= rxbuf_next_read_idx;// to prevent warning against volatile access order. 
    rxbuf_next_write_idx_var=rxbuf_next_write_idx;
    if ( rxbuf_next_read_idx_var == rxbuf_next_write_idx_var )
    {
        rv = -1;
    }
    else
    {
        rv = rxbuf[rxbuf_next_read_idx];
        rxbuf_next_read_idx = (rxbuf_next_read_idx + 1) & RX_BUFFER_DEPTH_MASK;
    }
    // END ATOMIC

    P1IE |= RXD;    // Enable interrupts for RX pin.

    return rv;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR (void)
{
  if (P1IFG & BIT3)  // interrupt p1.3 pin
  {
     if (programming_mode==0)
     {
       __delay_cycles(100);// so it does not trigger the switch flip noise. 
       init_programming_mode( );
       programming_mode=1;
       P1IES ^= BIT3;
     }
     else
     {
       programming_mode =0;
       P1IES ^= BIT3;

       play_knock_pattern(); 
       
       /*
       {
       	  bc_printf("Programming complete.\n New Knock Rhythm:");
	  int idx = 0;
	  while (idx < key_length_programmed)
       	    bc_printf(" %u ", KEY[idx++]);
       	  bc_printf("\n");
       }
       */
     }
     P1IFG &= ~BIT3; // P1.3 IFG cleared
    
  }
  else
  {
  
  
  unsigned char byte;

    P1IE &= ~RXD;   // Disable interrupts for RX pin.

#define IDEAL_BIT_PERIOD_USEC (104)
#define BIT_PD_FUDGE_FACTOR (64)    // measured empirically
#define BIT_PD_USEC \
    (IDEAL_BIT_PERIOD_USEC - BIT_PD_FUDGE_FACTOR )

#define INITIAL_DELAY_USEC (70)     // measured empirically

    // Assume that bit time is 104 microseconds (9600 baud)
    // Bits come LSB->MSB order.

    // Wait until middle of first data bit.
    //   (104 for start bit plus 104/2 to get
    //    to the middle of the first data bit)


    P1OUT ^= BIT5;  // (debug)
    delay_microseconds(INITIAL_DELAY_USEC);

    {
        int temp[8];

#define SAMPLE_AND_WAIT(idx)        \
    temp[idx] = P1IN;               \
    delay_microseconds(BIT_PD_USEC);
       
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(0);
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(1);
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(2);
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(3);
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(4);
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(5);
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(6);
        P1OUT ^= BIT5;  // (debug)
        SAMPLE_AND_WAIT(7);
        P1OUT ^= BIT5;  // (debug)

        byte = (temp[0] & RXD ? 0x01 : 0) |
               (temp[1] & RXD ? 0x02 : 0) |
               (temp[2] & RXD ? 0x04 : 0) |
               (temp[3] & RXD ? 0x08 : 0) |
               (temp[4] & RXD ? 0x10 : 0) |
               (temp[5] & RXD ? 0x20 : 0) |
               (temp[6] & RXD ? 0x40 : 0) |
               (temp[7] & RXD ? 0x80 : 0);
    }

    rxbuf[rxbuf_next_write_idx] = byte;

    // Save new byte in rx buffer.
    rxbuf_next_write_idx = (rxbuf_next_write_idx + 1) & RX_BUFFER_DEPTH_MASK;

    // If we just overwrote the end of the buffer, bump next read idx.
   unsigned char rxbuf_next_write_idx_var =0;
   rxbuf_next_write_idx_var =rxbuf_next_write_idx;
    if ( rxbuf_next_write_idx_var == rxbuf_next_read_idx )
        rxbuf_next_read_idx = (rxbuf_next_read_idx + 1) & RX_BUFFER_DEPTH_MASK;

    P1IFG &= ~RXD;  // Clear interrupt flag.
    P1IE |= RXD;    // Enable interrupts for RX pin.
    P1OUT ^= BIT5;  // (debug)
  }
}

