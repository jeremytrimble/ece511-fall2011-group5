#include "main.h"

#define THRESHOLD 550

unsigned long knock_amplitude = 0;
unsigned long timer = 0;

void initialize_adc()
{
  // Disable ADC just in case it was working before.
  ADC10CTL0 &= ~ENC;
  // we are using channel 5, with clock divided by 4.  ADC10CLK/4
  ADC10CTL1 = INCH_5 + ADC10DIV_4;
  // Vcc & Vss are reference for voltages for ADC to compare to.
  ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;

  // This sets the input channel 5 of the ADC to Pin P1.5 on the board.  
  ADC10AE0 |= BIT5;
  // Enable and start conversion
  ADC10CTL0 |= ENC + ADC10SC;
}

extern void init_detection()
{
  // ADC Input pin P1.5
  P1SEL |= BIT5;
  initialize_adc();
}

extern void run_detection()
{
  while(1)
  {  
    timer++;
   
   
    // Just in case the refrence voltage takes some time to settle, we let the
    // ADC wait before starting sampling
    // __delay_cycles(500);
   
    /* Lock out interrupts to avoid race condition when entering
     * low-power mode.  Without this, we might hang forever waiting for
     * an interrupt that has already occurred. */
    __disable_interrupt();

    ADC10CTL0 &= ~ADC10IFG;
    
    // Start ADC Conversion
    ADC10CTL0 |= ENC + ADC10SC;
   
    // Low power mode with interrupts enabled
    __bis_SR_register(CPUOFF + GIE);  // Interrupts reenabled here.

    // the value of the conversion is stored in ADC10MEM, we assign this value
    // to a variable
    knock_amplitude = ADC10MEM;
    if ((knock_amplitude > THRESHOLD) && (timer > 500))
    {
      if (programming_mode==1)
      {
        start_programming(timer);
      }
      else
      {
      report_knock(timer);
      }
      
      timer = 0;
      
    }
  }
}

// ADC10 interrupt service routine
#pragma vector = ADC10_VECTOR
__interrupt void adc10_isr()
{
  // Wake up processor and continue the while loop
  __bic_SR_register_on_exit(CPUOFF);
  // ------------------------------------
  //  or we can compare the value of the conversion here, and Call Recognize
  //  Function here
  // ------------------------------------
}

