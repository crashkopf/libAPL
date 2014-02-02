/*
  command.h
*/
#ifndef COMMAND_H
#define COMMAND_H

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PGM_P const char *
#define PROGMEM
#define strcmp_P strcmp
#endif

#define ARGC_MAX 16 //Maximum number of arguments

typedef void * environment;
typedef int (*vector)(int, char **, environment *);

typedef struct {
	vector function;
	PGM_P name;
	PGM_P description;
} command;

/*
typedef struct {
	unsigned int len;
	command cmd[];
} cmdmap;
*/

//int cmd_exec (char *, cmdmap *, environment *);
int cmd_exec (char *, command[], environment *);
void parse_ln (char *, int *, char **);
vector cmd_exists(char *, command[]);
//vector cmd_exists(char *, cmdmap *);

#endif /* COMMAND_H */
