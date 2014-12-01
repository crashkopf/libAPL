/*
  adcutils.c - shell utilities for ADC
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "shell/command.h"
#include "adcutils.h"

#include "driver/adc.h"

int adc (int argc, char * argv[], environment * env) {
	unsigned int chan;
	ADCsample_t sample;
	if (argc < 2) {
		printf_P(PSTR("Usage: %s <ADC channel>\r\n"), argv[0]);
		return 0;
	}
	chan = strtoul(argv[2], 0, 0);
	sample = ADC_Read(chan);
	printf_P(PSTR("%d\r\n"), sample);
	return 0;
}

int adcdump (int argc, char * argv[], environment * env) {
	unsigned int x,c;
	ADCsample_t sample;
	c = ADC_Channels();
	for (x = 0; x < c; x++) {
		sample = ADC_Read(x);
		printf_P(PSTR("%.2d - % 0d\r\n"), x, sample);
	}
	printf_P(PSTR("\r\n"));
	return 0;
}

int adcmon (int argc, char * argv[], environment * env) {
	unsigned int x,c;
	ADCsample_t sample;
	c = ADC_Channels();
	while (1) {
		for (x = 0; x < c; x++) {
			sample = ADC_Read(x);
			printf_P(PSTR("%.2d - % 0d\r\n"), x, sample);
		}
		printf_P(PSTR("\033];H"));
	}
	return 0;
}
