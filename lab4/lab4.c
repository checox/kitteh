#include <avr/io.h>
#include <util/delay.h>
char data_in;

void serial_init(unsigned short ubrr)
{
  UBRR0 = ubrr;           //set baud rate
  UCSR0B |= (1 << TXEN0); //turn on transmitter
  UCSR0B |= (1 << RXEN0); //turn on receiver
  UCSR0C = (3 << UCSZ00); //set for async operation, no parity, one stop bit, 8 data bits
}

void serial_out(char ch)
{
  while ((UCSR0A & (1<<UDRE0)) == 0);
  UDR0 = ch;
}

char serial_in()
{
  while(!(UCSR0A & (1 << RXC0)));
  return UDR0;
}


int main()
{
  serial_init(63);
  DDRC |= 1 << DDC0;
  PORTC &= ~(1<< PC0);

  while(1)
  {
    data_in = serial_in();
    serial_out(data_in);
  }
  return 0;
}

