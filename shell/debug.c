/*
  debug.c - shell commands for basic debugging
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "compat.h"
#include "shell/command.h"

#define DUMPCOL 16

int peek(int argc, char *argv[], environment * env) {
	union u {
		char * a;
		unsigned long l;
		unsigned char c[sizeof(char *)];
	};
	union u addr;
	unsigned char data;
	if (argc < 2) {
		printf_P(PSTR("Usage: %s address\r\n"), argv[0]);
		return 0;
	}
	addr.l = strtoul(argv[1], 0, 0);
	data = *addr.a;
	printf_P(PSTR("%x:%x:%c\r\n"), (unsigned int) addr.l, data, data);
	return 0;
}

int poke(int argc, char *argv[], environment * env) {
	union u {
		char * a;
		unsigned long l;
		unsigned char c[sizeof(char *)];
	};
	union u addr;
	unsigned char data;
	if (argc < 3) {
		printf_P(PSTR("Usage: %s address data\r\n"), argv[0]);
		return 0;
	}
	addr.l = strtoul(argv[1], 0, 0);
	data = strtoul(argv[2], 0, 0);
	*addr.a = data; 
	printf_P(PSTR("%x:%x\r\n"), (unsigned int) addr.l, data);
	return 0;
}

int dump(int argc, char *argv[], environment * env) {
	union u {
		char * a;
		unsigned long l;
		unsigned char c[sizeof(char *)];
	};
	union u addr;
	unsigned int len;
	if (argc < 3) {
		printf_P(PSTR("Usage: %s address length\r\n"), argv[0]);
		return 0;
	}
	addr.l = strtoul(argv[1], 0, 0);
	len = strtoul(argv[2], 0, 0);
	int col;
	while (len) {
		printf_P(PSTR("%.4x\t"), (unsigned int) addr.l);
		for (col = 0; col < DUMPCOL && len - col > 0; col++) {
			printf_P(PSTR("%.2x "), *(addr.a + col));
		}
		for (; col < DUMPCOL; col++) printf_P(PSTR("   "));
		printf_P(PSTR("\t"));
		for (col = 0; col < DUMPCOL && len - col > 0; col++) {
			if (((unsigned int) *(addr.a + col) > 31) && ((unsigned int) *(addr.a + col) < 127))
				printf_P(PSTR("%c"), *(addr.a + col));
			else
				printf_P(PSTR("."));
		}
		printf_P(PSTR("\r\n"));
		len -= col;
		addr.a += col;
	}
	return 0;
}
