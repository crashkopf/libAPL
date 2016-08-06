/*
  serio.c
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "serio.h"

SIO_port_t USART0 = {
	&UCSR0A,
	&UCSR0B,
	&UCSR0C,
	&UBRR0L,
	&UBRR0H,
	&UDR0
};

SIO_port_t USART1 = {
	&UCSR1A,
	&UCSR1B,
	&UCSR1C,
	&UBRR1L,
	&UBRR1H,
	&UDR1
};

SIO_port_t USART2 = {
	&UCSR2A,
	&UCSR2B,
	&UCSR2C,
	&UBRR2L,
	&UBRR2H,
	&UDR2
};

SIO_port_t USART3 = {
	&UCSR3A,
	&UCSR3B,
	&UCSR3C,
	&UBRR3L,
	&UBRR3H,
	&UDR3
};

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

ISR(USART0_RX_vect) {
	// Always read from UDR, otherwise the interrupt will keep firing
	char c = UDR0; 
	writeb(&USART0.rdbuf, &c, 1);
	if (USART0_rx) USART0_rx(&USART0, &UDR0);
}

ISR(USART0_TX_vect) {
	if (USART0_tx) USART0_tx(&USART0, &UDR0);
}

ISR(USART0_UDRE_vect) {
	char c;
	if (readb(&USART0.wrbuf, &c, 1) > 0) UDR0 = c;
	else txstop(&USART0);  // Turn off interrupt when the buffer is empty, otherwise it will keep firing
	if (USART0_dre) USART0_dre(&USART0, &UDR0);
}

void SIO_init(SIO_port_t * port, SIO_baud_t baud, unsigned char framedef) {
	
	SIO_set_baud(port, baud);
	SIO_set_frame(port, framedef);
	
	port->rdbuf.pos = 0;
	port->rdbuf.len = 0;
	port->rdbuf.size = BSIZE;
	port->rdbuf.data = port->rddata;

	port->wrbuf.pos = 0;
	port->wrbuf.len = 0;
	port->wrbuf.size = BSIZE;
	port->wrbuf.data = port->wrdata;
	
	rxstart(port); // enable interrupts
	txstart(port);
}

void SIO_set_baud(SIO_port_t *port, SIO_baud_t baud) {
	int use2x;
	unsigned long div;
	
	rxstop(port);
	txstop(port);
	
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
	
	rxstart(port);
	txstart(port);
}

void SIO_set_frame(SIO_port_t *port, unsigned char framedef) {

	rxstop(port);
	txstop(port);
	
	// This just sets us to 8N1.  I'll fix it later.
	*port->UCSRC = _BV(UCSZ0) | _BV(UCSZ1);

	rxstart(port);
	txstart(port);
}

int SIO_read(SIO_port_t *port, char * s, unsigned int m) {
	int n;
	rxstop(port);			// Stop RX so buffer isn't modified
	n = readb(&port->rdbuf, s, m);	// Read from buffer
	rxstart(port);			// Start RX again
	return n;
}

int SIO_write(SIO_port_t *port, char * s, unsigned int m) {
	int n;
	txstop(port);			// Stop TX so buffer isn't modified
	n = writeb(&port->wrbuf, s, m);	// Write to buffer
	txstart(port);			// Start TX again
	return n;
}

void SIO_set_rx_callback(SIO_port_t *port, SIO_rx_callback_t cb);
void SIO_set_tx_callback(SIO_port_t *port, SIO_tx_callback_t cb);
void SIO_set_dre_callback(SIO_port_t *port, SIO_dre_callback_t cb);
