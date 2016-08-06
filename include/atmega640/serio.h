#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef BAUD_TOL
#define BAUD_TOL 2
#endif

#include "buffer.h"

#ifndef BSIZE
#define BSIZE 64
#endif

// UCSRxA
#define U2X 1
#define TXEN 6
#define RXEN 7
// UCSRxB
#define UDRIE 5
#define TXCIE 6
#define RXCIE 7
// UCSRxC
#define UCSZ0 1
#define UCSZ1 2
#define USBS 3
#define UPM0 4
#define UPM1 5

typedef struct {
	volatile unsigned char *UCSRA;
	volatile unsigned char *UCSRB;
	volatile unsigned char *UCSRC;
	volatile unsigned char *UBRRL;
	volatile unsigned char *UBRRH;
	volatile unsigned char *UDR;
	buffer rdbuf;
	buffer wrbuf;
	char rddata[BSIZE];
	char wrdata[BSIZE];
} SIO_port_t;

typedef unsigned long SIO_baud_t;

extern SIO_port_t USART0;
extern SIO_port_t USART1;
extern SIO_port_t USART2;
extern SIO_port_t USART3;