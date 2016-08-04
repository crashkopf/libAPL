/*
  serio.c
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "serio.h"

char rdata[BSIZE];
char wdata[BSIZE];
buffer rdbuf = {0, 0, BSIZE, rdata};
buffer wrbuf = {0, 0, BSIZE, wdata};

SIO_rx_callback_t USART0_rx;
SIO_tx_callback_t USART0_tx;
SIO_dre_callback_t USART0_dre;

SIO_rx_callback_t USART1_rx;
SIO_tx_callback_t USART1_tx;
SIO_dre_callback_t USART1_dre;

SIO_rx_callback_t USART1_rx;
SIO_tx_callback_t USART1_tx;
SIO_dre_callback_t USART1_dre;

SIO_rx_callback_t USART2_rx;
SIO_tx_callback_t USART2_tx;
SIO_dre_callback_t USART2_dre;

static inline void txstart(SIO_port_t *port) {
	*port->UCSRB |= _BV(UDRIE) | _BV(TXEN);		// Turn on UDR empty interrupt
}
static inline void txstop(SIO_port_t *port) {
	*port->UCSRB &= ~(_BV(UDRIE) | _BV(TXEN));	// Turn off UDR empty interrupt
}
static inline void rxstart(SIO_port_t *port) {
	*port->UCSRB |= _BV(RXCIE) | _BV(RXEN);		// Turn on RX complete interrupt
}
static inline void rxstop(SIO_port_t *port) {
	*port->UCSRB &= ~(_BV(RXCIE) | _BV(RXEN));	// Turn off RX complete interrupt
}

void _rx_callback(volatile unsigned char * data) {
	char c = *data; 
	// Always read from UDR, otherwise the interrupt will keep firing
	writeb(&rdbuf, &c, 1);
}

void _tx_callback(volatile unsigned char * data) {
	char c = *data;
}

void _dre_callback(volatile unsigned char * data) {
	char c;
	int n;
	n = readb(&wrbuf, &c, 1);
	if (n > 0) UDR0 = c;
	else txstop();  // Turn off interrupt when the buffer is empty, otherwise it will keep firing
}

ISR(USART0_RX_vect) {
	USART0_rx(&UDR0);

}

ISR(USART0_TX_vect) {
	USART0_rx(&UDR0);

}

ISR(USART0_UDRE_vect) {
	USART0_dre(&UDR0);
}

void SIO_init(SIO_port_t * port, SIO_baud_t baud, SIO_frame_t frame) {
	
	SIO_set_baud(port, baud);
	SIO_set_frame(port, frame);

	rxstart(port); // enable interrupts
	txstart(port);

	
}

void SIO_set_baud(SIO_port_t *port, SIO_baud_t baud) {
	int use2x;
	unsigned long div;
	
	div = (((F_CPU) + 8UL * (baud)) / (16UL * (baud)) - 1UL);
	
	if (100 * (F_CPU) > (16 * ((div) + 1)) * (100 * (baud) + (baud) * (BAUD_TOL)))
		use2x=1;
	else if (100 * (F_CPU) < (16 * ((div) + 1)) * (100 * (baud) - (baud) * (BAUD_TOL)))
		use2x = 1;
	else
		use2x=0;

	if (use2x) {
		*port->UCSRA = _BV(U2X);             /* improve baud rate error by using 2x clk */
		*port->UBRRL = (F_CPU / (8UL * baud)) - 1;
	}
	else
		*port->UBRRL = (F_CPU / (16UL * baud)) - 1;
}

void SIO_set_frame(SIO_port_t *port, SIO_frame_t frame) {
	unsigned char fbits = 0;
	
	fbits |= (frame.data & 1) << UCSZ0;
	
	*port->UCSRC = ((frame.data & 3)<<UCSZ0) 
		| (frame.parity << UPM0)
		| (frame.stop << USBS)
	;
	
}

/*
int SIO_read(SIO_port_t *port, const unsigned char * s, unsigned int m) {
	int n;
	rxstop(port);			// Stop RX so buffer isn't modified
	n = readb(&rdbuf, s, m);	// Read from buffer
	rxstart(port);			// Start RX again
	return n;
}

int SIO_write(SIO_port_t *port, const unsigned char * s, unsigned int m) {
	int n;
	txstop(port);			// Stop TX so buffer isn't modified
	n = writeb(&wrbuf, s, m);	// Write to buffer
	txstart(port);			// Start TX again
	return n;
}
*/

int SIO_set_rx_callback(SIO_port_t *port, SIO_rx_callback_t cb);
int SIO_set_tx_callback(SIO_port_t *port, SIO_tx_callback_t cb);
int SIO_set_dre_callback(SIO_port_t *port, SIO_dre_callback_t cb);

/*
 These functions will IO block and are only for debugging.
 */
/*
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
*/