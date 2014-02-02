/*
  buffer.h - buffer handling utilities for stacks and FIFOs
*/

//#define BSIZE 8

typedef struct {
	unsigned int pos;  // The offset of the head
	unsigned int len;  // How much data is in the buffer
	unsigned int size;
	char * data;
} buffer;

int readb(buffer *, char *, unsigned int);
int writeb(buffer *, char *, unsigned int);
int bflush(buffer *);
