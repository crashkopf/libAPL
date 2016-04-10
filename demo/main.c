#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "driver/serio.h"
#include "sys/device.h"
#include "sys/proc.h"

static int sputc (char c, FILE *stream) {
	while (SIO_write(&c, 1) < 1); 
	return 0;
}

static int sgetc (FILE *stream) {
	char c;
	while (SIO_read(&c, 1) < 1); 
	return c;
}

static FILE sio0 = FDEV_SETUP_STREAM(sputc, sgetc, _FDEV_SETUP_RW);

void main (void) {	
	// Set up serial I/O
	SIO_init(115200);
	stdin=&sio0;
	stdout=&sio0;
	
	// Turn on sleep
	sleep_enable();
	
	// Enable interrrupts
	sei();

	while(1)
		puts("\r\nFUCK");

}