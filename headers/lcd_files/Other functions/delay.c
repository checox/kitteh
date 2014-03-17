// delay.c
// for NerdKits with ATmega168, 14.7456 MHz clock
// mrobbins@mit.edu

#include <inttypes.h>
#include "delay.h"

// delay_us(...):
// delays a given number of microseconds
inline void delay_us(uint16_t us) {
  //_delay_us(us);
  uint16_t i;
  for(i=0; i<us; i++) {
    NOP;	// two is right for 8MHz, tested by mikey and robtruax
    NOP;
    
    NOP;	// so adding 7 more will yield a slightly slow clock (1.017us)
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
  }
}

// delay_ms(...):
// delays a given number of milliseconds
void delay_ms(uint16_t ms) {
  //_delay_ms(ms);
  uint16_t i;
  for(i=0; i<ms; i++)
    delay_us(1000);
}

//delays number of seconds
//cumalitive error is ~5 second every minute
void delay_s(uint16_t s){
	uint16_t i;
	for(i=0; i<s; i++)
		delay_ms(1000);
}