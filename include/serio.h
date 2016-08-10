/*
  serio.h
*/

#define xstringify(x) #x
#define stringify(x) xstringify(x)
#define pathinclude(path, file) stringify(path/file)

/*
#ifdef _PROJECT
#define inc stringify(_PROJECT/serio.h)
#include inc
#undef inc
#endif
*/

typedef struct SIO_port SIO_port_t;

#ifdef _TARGET
#define inc stringify(_TARGET/serio.h)
#include inc
#undef inc
#endif
#ifdef _CPU
#define inc stringify(_CPU/serio.h)
#include inc
#undef inc
#endif
#ifdef _ARCH
#define inc stringify(_ARCH/serio.h)
#include inc
#undef inc
#endif

#define SIO_8N1 0

#define SIO_DATABITS_8 3
#define SIO_PARITY_NONE 0
#define SIO_STOPBITS_1 0

typedef void (* SIO_rx_callback_t)(SIO_port_t * port);
typedef void (* SIO_tx_callback_t)(SIO_port_t * port);
typedef void (* SIO_dre_callback_t)(SIO_port_t * port);

void SIO_init(SIO_port_t * port, SIO_baud_t baud, unsigned char framedef);
void SIO_set_baud(SIO_port_t *port, SIO_baud_t baud);
void SIO_set_frame(SIO_port_t *port, unsigned char framedef);

int SIO_read(SIO_port_t *port, char * s, unsigned int m);
int SIO_write(SIO_port_t *port, char * s, unsigned int m);

void SIO_set_rx_callback(SIO_port_t *port, SIO_rx_callback_t cb);
void SIO_set_tx_callback(SIO_port_t *port, SIO_tx_callback_t cb);
void SIO_set_dre_callback(SIO_port_t *port, SIO_dre_callback_t cb);