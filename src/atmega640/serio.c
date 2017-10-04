/*
  serio.c
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "serio.h"

typedef struct SIO_port {
	volatile unsigned char *UCSRA;
	volatile unsigned char *UCSRB;
	volatile unsigned char *UCSRC;
	volatile unsigned char *UBRRL;
	volatile unsigned char *UBRRH;
	volatile unsigned char *UDR;
	volatile unsigned char *RTS_port;
	volatile unsigned char *RTS_DDR;
	unsigned char RTS_pin;
	volatile unsigned char *CTS_port;
	volatile unsigned char *CTS_DDR;
	unsigned char CTS_pin;
	SIO_rx_callback_t rx_callback;
	SIO_tx_callback_t tx_callback;
	SIO_dre_callback_t dre_callback;
	buffer rdbuf;
	buffer wrbuf;
	char rddata[BSIZE];
	char wrdata[BSIZE];
} SIO_port_t;

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

static inline void SIO_txstart(SIO_port_t *port) {
	*port->UCSRB |= _BV(UDRIE) | _BV(TXEN);		// Turn on UDR empty interrupt
}
static inline void SIO_txstop(SIO_port_t *port) {
	*port->UCSRB &= ~(_BV(UDRIE) | _BV(TXEN));	// Turn off UDR empty interrupt
}
static inline void SIO_rxstart(SIO_port_t *port) {
	*port->UCSRB |= _BV(RXCIE) | _BV(RXEN);		// Turn on RX complete interrupt
}
static inline void SIO_rxstop(SIO_port_t *port) {
	*port->UCSRB &= ~(_BV(RXCIE) | _BV(RXEN));	// Turn off RX complete interrupt
}

// USART0 interrupt handlers
ISR(USART0_RX_vect) {
	// Always read from UDR, otherwise the interrupt will keep firing
	char c = UDR0; 
	writeb(&USART0.rdbuf, &c, 1);
	if (USART0.rx_callback) USART0.rx_callback(&USART0);
}

ISR(USART0_TX_vect) {
	if (USART0.tx_callback) USART0.tx_callback(&USART0);
}

ISR(USART0_UDRE_vect) {
	char c;
	if (readb(&USART0.wrbuf, &c, 1) > 0) UDR0 = c;
	else SIO_txstop(&USART0);  // Turn off interrupt when the buffer is empty, otherwise it will keep firing
	if (USART0.dre_callback) USART0.dre_callback(&USART0);
}

// USART1 interrupt handlers
ISR(USART1_RX_vect) {
	// Always read from UDR, otherwise the interrupt will keep firing
	char c = UDR1; 
	writeb(&USART1.rdbuf, &c, 1);
	if (USART1.rx_callback) USART1.rx_callback(&USART1);
}

ISR(USART1_TX_vect) {
	if (USART1.tx_callback) USART1.tx_callback(&USART0);
}

ISR(USART1_UDRE_vect) {
	char c;
	if (readb(&USART1.wrbuf, &c, 1) > 0) UDR1 = c;
	else SIO_txstop(&USART1);  // Turn off interrupt when the buffer is empty, otherwise it will keep firing
	if (USART1.dre_callback) USART1.dre_callback(&USART1);
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
	
	SIO_rxstart(port); // enable interrupts
	SIO_txstart(port);
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

void SIO_set_frame(SIO_port_t *port, unsigned char framedef) {
	// This just sets us to 8N1.  I'll fix it later.
	*port->UCSRC = _BV(UCSZ0) | _BV(UCSZ1);
}

int SIO_read(SIO_port_t *port, char * s, unsigned int m) {
	int n;
	SIO_rxstop(port);			// Stop RX so buffer isn't modified
	n = readb(&port->rdbuf, s, m);	// Read from buffer
	SIO_rxstart(port);			// Start RX again
	return n;
}

int SIO_write(SIO_port_t *port, char * s, unsigned int m) {
	int n;
	SIO_txstop(port);			// Stop TX so buffer isn't modified
	n = writeb(&port->wrbuf, s, m);	// Write to buffer
	SIO_txstart(port);			// Start TX again
	return n;
}

void SIO_set_rx_callback(SIO_port_t *port, SIO_rx_callback_t cb) {
	port->rx_callback = cb;
}
void SIO_set_tx_callback(SIO_port_t *port, SIO_tx_callback_t cb) {
	port->tx_callback = cb;
}
void SIO_set_dre_callback(SIO_port_t *port, SIO_dre_callback_t cb) {
	port->dre_callback = cb;
}
