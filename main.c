/*************************************************************
*       at328-5.c - Demonstrate interface to a parallel LCD display
*
*       This program will print a message on an LCD display
*       using the 4-bit wide interface method
*
*       PORTB, bit 4 (0x10) - output to RS (Register Select) input of display
*              bit 3 (0x08) - output to R/W (Read/Write) input of display
*              bit 2 (0x04) - output to E (Enable) input of display
*       PORTB, bits 0-1, PORTD, bits 2-7 - Outputs to DB0-DB7 inputs of display.
*
*       The second line of the display starts at address 0x40.
*
* Revision History
* Date     Author      Description
* 11/17/05 A. Weber    First Release for 8-bit interface
* 01/13/06 A. Weber    Modified for CodeWarrior 5.0
* 08/25/06 A. Weber    Modified for JL16 processor
* 05/08/07 A. Weber    Some editing changes for clarification
* 06/25/07 A. Weber    Updated name of direct page segment
* 08/17/07 A. Weber    Incorporated changes to demo board
* 01/15/08 A. Weber    More changes to the demo board
* 02/12/08 A. Weber    Changed 2nd LCD line address
* 04/22/08 A. Weber    Added "one" variable to make warning go away
* 04/15/11 A. Weber    Adapted for ATmega168
* 01/30/12 A. Weber    Moved the LCD strings into ROM
* 02/27/12 A. Weber    Corrected port bit assignments above
* 11/18/13 A. Weber    Renamed for ATmega328P
*************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

/*
  The NIBBLE_HIGH condition determines whether data bits 4-7 or 0-3 are used
  to send the four bit nibble to the LCD.
  If NIBBLE_HIGH is declared, use data bits 4-7.
  If NIBBLE_HIGH not declared, use data bits 0-3.
*/
#define NIBBLE_HIGH                 // Use bits 4-7 for talking to LCD

void initialize(void);
void strout(int, unsigned char *);
void cmdout(unsigned char, unsigned char);
void datout(unsigned char);
void nibout(unsigned char, unsigned char);
void busywt(void);

/*
  Use the "PROGMEM" attribute to store the strings in the ROM
  insteat of in RAM.
*/
#ifdef NIBBLE_HIGH
const unsigned char str1[] PROGMEM = ">> at328-5.c hi <<901234";
#else
const unsigned char str1[] PROGMEM = ">> at328-5.c lo <<901234";
#endif
const unsigned char str2[] PROGMEM = ">> USC EE459L <<78901234";

#define LCD_RS          0x08//PC3
#define LCD_RW          0x10//PC4
#define LCD_E           0x20//PC5
#define LCD_Bits        (LCD_RS|LCD_RW|LCD_E)

#ifdef NIBBLE_HIGH
#define LCD_Data_D     0xf0     // PD4-7 Bits in Port D for LCD data
#define LCD_Status     0x80     // Bit in Port D for LCD busy status
#else
#define LCD_Data_B     0x03     // Bits in Port B for LCD data
#define LCD_Data_D     0x0c     // Bits in Port D for LCD data
#define LCD_Status     0x08     // Bit in Port D for LCD busy status
#endif

#define WAIT           1
#define NOWAIT         0

int main(void) {
    unsigned char one = 1;

#ifdef NIBBLE_HIGH
    DDRD |= LCD_Data_D;         // Set PORTD bits 4-7 for output
#else
    DDRB |= LCD_Data_B;         // Set PORTB bits 0-1 for output
    DDRD |= LCD_Data_D;         // Set PORTD bits 2-3 for output
#endif

    DDRC |= LCD_Bits;           // Set PORTB bits 2, 3 and 4 for output
    
    initialize();               // Initialize the LCD display

    _delay_ms(2000);
    strout(0, (unsigned char *) str1);    // Print string on line 1

    strout(0x40, (unsigned char *) str2); // Print string on line 2

    while (one) {               // Loop forever
    PORTD |= 0x80; // Set E to 1
    PORTD &= ~0x80;            // Set E to 0

    }


    return 0;   /* never reached */
}

/*
  strout - Print the contents of the character string "s" starting at LCD
  RAM location "x".  The string must be terminated by a zero byte.
*/
void strout(int x, unsigned char *s)
{
    unsigned char ch;

    cmdout(x | 0x80, WAIT);     // Make A contain a Set Display Address command

    /* Use the "pgm_read_byte()" routine to read the date from ROM */
    while ((ch = pgm_read_byte(s++)) != (unsigned char) '\0') {
        datout(ch);             // Output the next character
    }
}

/*
  datout - Output a byte to the LCD display data register (the display)
  and wait for the busy flag to reset.
*/
void datout(unsigned char x)
{
    PORTC &= ~(LCD_RW|LCD_E);   // Set R/W=0, E=0, RS=1
    PORTC |= LCD_RS;
    nibout(x, NOWAIT);
    nibout(x << 4, WAIT);
}

/*
  cmdout - Output a byte to the LCD display instruction register.  If
  "wait" is non-zero, wait for the busy flag to reset before returning.
  If "wait" is zero, return immediately since the BUSY flag isn't
  working during initialization.
*/
void cmdout(unsigned char x, unsigned char wait)
{
    PORTC &= ~LCD_Bits;         // Set R/W=0, E=0, RS=0
    nibout(x, NOWAIT);
    nibout(x << 4, wait);
}

/*
  nibout - Puts bits 4-7 from x into the four bits that we're
  using to talk to the LCD.  The other bits of the port are unchanged.
  Toggle the E control line low-high-low.
*/
void nibout(unsigned char x, unsigned char wait)
{
#ifdef NIBBLE_HIGH
    PORTD |= (x & LCD_Data_D);  // Put high 4 bits of data in PORTD
    PORTD &= (x | ~LCD_Data_D);
#else
    PORTB |= (x & LCD_Data_B);  // Put low 2 bits of data in PORTB
    PORTB &= (x | ~LCD_Data_B);
    PORTD |= (x & LCD_Data_D);  // Put high 2 bits of data in PORTD
    PORTD &= (x | ~LCD_Data_D);
#endif
    PORTC |= LCD_E;             // Set E to 1
    PORTC &= ~LCD_E;            // Set E to 0
    if (wait)
        busywt();               // Wait for BUSY flag to reset
}

/*
  initialize - Do various things to force a initialization of the LCD
  display by instructions, and then set up the display parameters and
  turn the display on.
*/
void initialize()
{
    _delay_ms(15);              // Delay at least 15ms

    nibout(0x30, NOWAIT);       // Send a 0x30
    _delay_ms(4);               // Delay at least 4msec

    nibout(0x30, NOWAIT);       // Send a 0x30
    _delay_us(120);             // Delay at least 100usec

    nibout(0x30, NOWAIT);       // Send a 0x30

    nibout(0x20, WAIT);         // Function Set: 4-bit interface
    
    cmdout(0x28, WAIT);         // Function Set: 4-bit interface, 2 lines

    cmdout(0x0f, WAIT);         // Display and cursor on
}

/*
  busywt - Wait for the BUSY flag to reset
*/
void busywt()
{
    unsigned char bf;

#ifdef NIBBLE_HIGH
    PORTD &= ~LCD_Data_D;       // Set for no pull ups
    DDRD &= ~LCD_Data_D;        // Set for input
#else
    PORTB &= ~LCD_Data_B;       // Set for no pull ups
    PORTD &= ~LCD_Data_D;
    DDRB &= ~LCD_Data_B;        // Set for input
    DDRD &= ~LCD_Data_D;
#endif

    PORTC &= ~(LCD_E|LCD_RS);   // Set E=0, R/W=1, RS=0
    PORTC |= LCD_RW;

    do {
        PORTC |= LCD_E;         // Set E=1
        _delay_us(1);           // Wait for signal to appear
        bf = PIND & LCD_Status; // Read status register high bits
        PORTC &= ~LCD_E;        // Set E=0
	PORTC |= LCD_E;         // Need to clock E a second time to fake
	PORTC &= ~LCD_E;        //   getting the status register low bits
    } while (bf != 0);          // If Busy (PORTD, bit 7 = 1), loop

#ifdef NIBBLE_HIGH
    DDRD |= LCD_Data_D;         // Set PORTD bits for output
#else
    DDRB |= LCD_Data_B;         // Set PORTB, PORTD bits for output
    DDRD |= LCD_Data_D;
#endif
}
