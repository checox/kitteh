#ifndef ADC_HEADERS_H
#define ADC_HEADERS_H

#include <avr/io.h>

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

#endif /*ADC_HEADERS_H*/


