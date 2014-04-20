/*************************************************************
* TEAM KITTEH MAIN CODE 
*************************************************************/
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "./headers/lcd_header.h"
#include "./headers/adc_headers.h"

void setup();

const unsigned char str1[] PROGMEM = "SAMSUNG LAWN MANAGER";
const unsigned char str2[] PROGMEM = "our lcd works";


int main(void) 
{
  setup();
    uint16_t last_sample = 0;
    double this_sample;
    double sample_avg;
    uint8_t i; 
    

    for(;;) 
    {               // Loop forever
      sample_avg =0.0;
      for (i=0; i<100; i++)
      {
        last_sample = adc_read();
        //maybe add a conversion function here
        sample_avg += this_sample/100.0;
      }
      unsigned char str_adc_sample[40];
      //sprintf(str_adc_sample, "Moisture = %" PRIu16, sample_avg); 
      //strout(0x40, (unsigned char *) str_adc_sample);
    }
  return 0;   /* never reached */
}

void setup()
{
  //enable global interrupts
  sei();
  DDRD |= 1<<PIND2 | 1<<PIND3;
  adc_init();
  lcd_initialize();               // Initialize the LCD display
  strout(0, (unsigned char *) str1);    // Print string on line 1
  strout(0x40, (unsigned char *) str2); // Print string on line 2

}
