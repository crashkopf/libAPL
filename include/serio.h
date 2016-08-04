/*
  serio.h
*/

#define xstringify(x) #x
#define stringify(x) xstringify(x)
#define pathinclude(path, file) stringify(path/file)

#ifdef _ARCH
#define inc stringify(_ARCH/serio.h)
#include inc
#undef inc
#endif
#ifdef _CPU
#define inc stringify(_CPU/serio.h)
#include inc
#undef inc
#endif
#ifdef _TARGET
#define inc stringify(_TARGET/serio.h)
#include inc
#undef inc
#endif
/*
#ifdef _PROJECT
#define inc stringify(_PROJECT/serio.h)
#include inc
#undef inc
#endif
*/

typedef (* SIO_rx_callback_t)(volatile unsigned char * data);
typedef (* SIO_tx_callback_t)(volatile unsigned char * data);
typedef (* SIO_dre_callback_t)(volatile unsigned char * data);

void SIO_init(SIO_port_t * port, SIO_baud_t baud, SIO_frame_t frame);
void SIO_set_baud(SIO_port_t *port, SIO_baud_t baud);
void SIO_set_frame(SIO_port_t *port, SIO_frame_t frame);

int SIO_read(SIO_port_t *port, const unsigned char * s, unsigned int m);
int SIO_write(SIO_port_t *port, const unsigned char * s, unsigned int m);

int SIO_set_rx_callback(SIO_port_t *port, SIO_rx_callback_t cb);
int SIO_set_tx_callback(SIO_port_t *port, SIO_tx_callback_t cb);
int SIO_set_dre_callback(SIO_port_t *port, SIO_dre_callback_t cb);