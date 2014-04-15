// main function for the project

#define F_CPU 9830000

#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include <util/delay.h>
#include "./headers/lcd_files/lcd.h"
#include <avr/eeprom>

void adc_init()
{
  //set analog to digital converter
  ADMUX=0;
  //set analog to digital converter to be enabled with a clock prescale of /
  ADSCRA = (1<<ADEN) | (1<<ADPS2)
int main() {
  //put our main code here
}

