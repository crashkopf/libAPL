/*
  serio.c
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "driver/serio.h"
#include "util/buffer.h"
#include "sys/device.h"

#ifndef BAUD_TOL
  #define BAUD_TOL 2
#endif
#define BSIZE 64


char rdata[BSIZE];
char wdata[BSIZE];
buffer rdbuf = {0, 0, BSIZE, rdata};
buffer wrbuf = {0, 0, BSIZE, wdata};

static inline void txstart(void) {
	UCSR0B |= _BV(UDRIE0);		// Turn on UDR empty interrupt
}
static inline void txstop(void) {
	UCSR0B &= ~(_BV(UDRIE0));	// Turn off UDR empty interrupt
}
static inline void rxstart(void) {
	UCSR0B |= _BV(RXCIE0);		// Turn on RX complete interrupt
}
static inline void rxstop(void) {
	UCSR0B &= ~(_BV(RXCIE0));	// Turn off RX complete interrupt
}

ISR(USART0_RX_vect) {
	char c;
	//int n;
	c = UDR0; // Always read from UDR, otherwise the interrupt will keep firing
	//n = writeb(&rdbuf, &c, 1);
	writeb(&rdbuf, &c, 1);
}

ISR(USART0_UDRE_vect) {
	char c;
	int n;
	n = readb(&wrbuf, &c, 1);
	if (n > 0) UDR0 = c;
	else txstop();  // Turn off interrupt when the buffer is empty, otherwise it will keep firing
}

int SIO_read(char * s, unsigned int m) {
	int n;
	rxstop();			// Stop RX so buffer isn't modified
	n = readb(&rdbuf, s, m);	// Read from buffer
	rxstart();			// Start RX again
	return n;
}

int SIO_write(char * s, unsigned int m) {
	int n;
	txstop();			// Stop TX so buffer isn't modified
	n = writeb(&wrbuf, s, m);	// Write to buffer
	txstart();			// Start TX again
	return n;
}

/*
 * Initialize the UART
 * Sets up as baud N81
 */
void SIO_init(unsigned long baud) {
	int use2x;
	unsigned long div = (((F_CPU) + 8UL * (baud)) / (16UL * (baud)) - 1UL);
	
	if (100 * (F_CPU) > (16 * ((div) + 1)) * (100 * (baud) + (baud) * (BAUD_TOL)))
		use2x=1;
	else if (100 * (F_CPU) < (16 * ((div) + 1)) * (100 * (baud) - (baud) * (BAUD_TOL)))
		use2x = 1;
	else
		use2x=0;

	if (use2x) {
		UCSR0A = _BV(U2X0);             /* improve baud rate error by using 2x clk */
		UBRR0L = (F_CPU / (8UL * baud)) - 1;
	}
	else
		UBRR0L = (F_CPU / (16UL * baud)) - 1;

	UCSR0B |= _BV(TXEN0) | _BV(RXEN0); /* tx/rx enable */

	rxstart(); // enable interrupts
	txstart();

	UCSR0C = (3<<UCSZ00); // 8-bit data (default)
}

/*
 These functions will IO block and are only for debugging.
 */
int uart_putc(char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0);  // Wait until data register is empty
  UDR0 = c;	// Load a character into the register

  return 0;
}

char uart_getc(void) {
  loop_until_bit_is_set(UCSR0A, RXC0);
  return UDR0;
}

int uart_puts(char * s) {
	while (*s != '\0') {
		if (*s == '\n') uart_putc('\r');
		uart_putc(*s);
		s++;
	}
	return 0;
}
