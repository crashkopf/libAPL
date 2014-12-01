/*
  unitest.c - test your unit
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "adc.h"
#include "adcmap.h"
#include "serio.h"
#include "event.h"
#include "shell/shell.h"

char peektest[]={0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};

char lnbuf[128];

static int sputc (char c, FILE *stream) {
	//uart_putc(c); return 0;
	//if (SIO_write(&c, 1) == 1) return 0;
	//else return 1;
	while (SIO_write(&c, 1) < 1); 
	return 0;
}
static int sgetc (FILE *stream) {
	//return uart_getc();
	char c;
	while (SIO_read(&c, 1) < 1); 
	return c;
}

void main (void) {	
	int mapsize;
	ADCcmap_t * mapptr;
	
	SIO_init();
	ADC_Init();

	sleep_enable();
	sei();

	mapsize = sizeof(ADCcmap_GCU_v303_full.len) + (sizeof(ADCdata_t) * pgm_read_word(&ADCcmap_GCU_v303_full.len));
	mapptr = malloc(mapsize);
	ADC_loadmap(mapptr, &ADCcmap_GCU_v303_full, mapsize);

	fdevopen(sputc, sgetc);
	
	puts("\r\nGCU Shell 0.1");

	while (1) {
		char * ln;
		ln = readline ("> ");
		if (strlen(ln) && Shell_exec(ln) == ~0) {
			printf("Command not found\n");
		}
		free(ln);
	}	
	
}
