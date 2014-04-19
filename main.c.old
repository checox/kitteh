// main function for the project

//#define F_CPU 9830000

#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include <util/delay.h>
#include "./headers/lcd_files/lcd.h"
//#include <avr/eeprom>

void adc_init()
{
  //set analog to digital converter for PC2 input
  ADMUX= (1<<MUX1);
  //set analog to digital converter to be enabled with a clock prescale of /
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
  //We want ours to be in free running mode so dont touch ADSCRB
  //Fire initial conversion to get ADC warmed up
  ADCSRA |= (1<<ADSC);
  //Disable digital input buffer on ADC pin C2 in order to save power
  DIDR0 = (1<<ADC2D);
}

uint16_t adc_read()
{
  //read from ADC, wait for conversion to finish
  //the micro will set ADSC to 0 when conversion is finished
  while(ADCSRA & (1<<ADSC))
  {
    //patience
  }
  //ADCL and ADCH are the registers that store the 16 bit value of the conversion
  uint16_t result = ADCL;
  uint16_t temp = ADCH;
  //combine the low and high result into one
  result = result + (temp<<8);

  //set ADSC bit to get the next conversion started
  ADCSRA |= (1<<ADSC);

  return result;
}

void setup()
{
  //enable global interrupts
  sei();
  DDRD |= 1<<PIND2 | 1<<PIND3;
}

int main() 
{
  //lcd_init();
  FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
  //lcd_home();

  adc_init();

  //start up the serial port
  //uart_init();
  //FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
  //stdin = stdout = &uart_stream;

  //holder variables for ADC value
  uint16_t last_sample = 0;
  double this_sample;
  double sample_avg;
  uint8_t i; 
  
  //the main body of the code
  while(1)
  {
    sample_avg =0.0;
    for (i=0; i<100; i++)
    {
      last_sample = adc_read();
      //maybe add a conversion function here

      sample_avg += this_sample/100.0;
    }
  }
}

