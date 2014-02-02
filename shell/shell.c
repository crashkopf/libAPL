/* shell.c */

#include <stdio.h>
#include <stdlib.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#include "fakeavr.h"
#endif //__AVR__
#include "shell/command.h"
#include "shell/shell.h"


#include "debug.h"
#include "adcutils.h"

int echo (int, char **, environment *);
int help(int, char **, environment *);

static const char echo_str [] PROGMEM = "echo";
static const char echo_desc [] PROGMEM = "Print command line arguments"; 
static const char help_str [] PROGMEM = "help";
static const char help_desc [] PROGMEM = "Show list of available commands";
static const char quest_str [] PROGMEM = "?";
static const char quest_desc [] PROGMEM = "Same as 'help'";  

#define INCLUDE_CMDS \
	INCLUDE_DEBUG \
	INCLUDE_ADCUTILS

#define V(c) static const char c##_str [] PROGMEM = #c; 
	INCLUDE_CMDS
#undef V

#define V(c) {&c, c##_str, c##_desc},
command GCUShell[] = {
	INCLUDE_CMDS 
	{&echo, echo_str, echo_desc}, 
	{&help, help_str, help_desc}, 
	{&help, quest_str, quest_desc}, 
	{NULL,NULL}
};
#undef V

#define LINEBUF_SIZE 128

// Some day (sigh)...
//int ls (int argc, char * argv[], environment * env) {return 0;}

// This function is based on the standard "echo" utility
int echo (int argc, char * argv[], environment * env) {
		int i;
		for (i = 1; i < argc; ) {
			printf_P(PSTR("%s"), argv[i]);
			if (++i < argc)
				printf_P(PSTR(" "));
		}
		printf_P(PSTR("\r\n"));
	return 0;
}

int help(int argc, char *argv[], environment * env) {
	unsigned int x = 0;
	printf_P(PSTR("Available commands:\r\n"));
	while (GCUShell[x].name != NULL) {
		printf_P(PSTR("\t%S \t- %S\r\n"), GCUShell[x].name, GCUShell[x].description);
		x++;
	}
	return 0;
}

void Shell_init(FILE * in, FILE * out, FILE * err) {
	
}

int Shell_exec  (char * str) {
	return cmd_exec(str, GCUShell, NULL);
}

char * readline (char * prompt) {
	char * lnbuf;
	unsigned int lbp = 0;
	int EOI = 0;
	lnbuf = malloc(LINEBUF_SIZE);
	if (!lnbuf) return NULL;
	fputs(prompt, stdout);
	while (!EOI) {
		char c;
		c = fgetc(stdin);
		switch (c) {
		case '\r':
		case '\n':
			fputc('\r', stdout);
			fputc('\n', stdout);
			EOI = 1;
			break;
		case '\b':
		case (char) 0x7f:
			if(lbp > 0) {
				lbp--;
				fputs("\b \b", stdout);
			}
			break;
		default:
			if (lbp < LINEBUF_SIZE - 1) {
				lnbuf[lbp++] = c;
				fputc(c, stdout);
			}
			else fputc('\a', stdout);
			break;
		}
	}
	lnbuf[lbp] = '\0';
	return lnbuf;
}
