/*
  debug.c - shell commands for basic debugging
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "shell/command.h"

#define DUMPCOL 16

int peek(int argc, char *argv[], environment * env) {
	char * addr;
	unsigned char data;
	if (argc < 2) {
		printf_P(PSTR("Usage: %s address\r\n"), argv[0]);
		return 0;
	}
	addr = (char *) (unsigned int) strtoul(argv[1], 0, 0);
	data = *addr;
	printf_P(PSTR("%x:%x:%c\r\n"), (unsigned int) addr, data, data);
	return 0;
}

int poke(int argc, char *argv[], environment * env) {
	char * addr;
	unsigned char data;
	if (argc < 3) {
		printf_P(PSTR("Usage: %s address data\r\n"), argv[0]);
		return 0;
	}
	addr = (char *) (unsigned int) strtoul(argv[1], 0, 0);
	data = strtoul(argv[2], 0, 0);
	*addr = data; 
	printf_P(PSTR("%x:%x\r\n"), (unsigned int) addr, data);
	return 0;
}

int dump(int argc, char *argv[], environment * env) {
	unsigned char * addr;
	unsigned int len;
	if (argc < 3) {
		printf_P(PSTR("Usage: %s address length\r\n"), argv[0]);
		return 0;
	}
	addr = (unsigned char *) (unsigned int) strtoul(argv[1], 0, 0);
	len = strtoul(argv[2], 0, 0);
	int col;
	while (len) {
		printf_P(PSTR("%.4x\t"), (unsigned int) addr);
		for (col = 0; col < DUMPCOL && len - col > 0; col++) {
			printf_P(PSTR("%.2x "), *(addr + col));
		}
		for (; col < DUMPCOL; col++) printf_P(PSTR("   "));
		printf_P(PSTR("\t"));
		for (col = 0; col < DUMPCOL && len - col > 0; col++) {
			if (((unsigned int) *(addr + col) > 31) && ((unsigned int) *(addr + col) < 127))
				printf_P(PSTR("%c"), *(addr + col));
			else
				printf_P(PSTR("."));
		}
		printf_P(PSTR("\r\n"));
		len -= col;
		addr += col;
	}
	return 0;
}
