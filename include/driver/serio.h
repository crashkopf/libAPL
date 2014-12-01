/*
  serio.h
*/

void SIO_init(unsigned long);
int SIO_read(char *, unsigned int);
int SIO_write(char *, unsigned int);

// Only use these functions for debugging, since they block processing
char uart_getc(void);
int uart_putc(char);
int uart_puts(char *);
