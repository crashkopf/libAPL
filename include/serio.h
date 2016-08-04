/*
  serio.c
*/

#ifdef _ARCH
#include ARCH/serio.h
#endif
#ifdef _DEVICE
#include ARCH/serio.h
#endif
#ifdef _TARGET
#include ARCH/serio.h
#endif

typedef struct SIO_port SIO_port_t;

typedef unsigned long SIO_baud_t;

typedef struct {
	unsigned char data : 3;
	unsigned char parity : 2;
	unsigned char stop : 1;
} SIO_frame_t;

typedef (* SIO_rx_callback_t)(volatile unsigned char * data);

typedef (* SIO_tx_callback_t)(volatile unsigned char * data);

typedef (* SIO_dre_callback_t)(volatile unsigned char * data);

int SIO_read(SIO_port_t *port, const unsigned char * s, unsigned int m);

int SIO_write(SIO_port_t *port, const unsigned char * s, unsigned int m);

void SIO_set_baud(SIO_port_t *port, SIO_baud_t baud);

void SIO_set_frame(SIO_port_t *port, SIO_frame_t frame);

void SIO_init(SIO_port_t * port, SIO_baud_t baud, SIO_frame_t frame);
