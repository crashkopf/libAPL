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
#define CMDBLK 8
#define NAME_MAX 16 //Maximum name length

typedef void * environment;
typedef int (*vector)(int, char **, environment *);

typedef struct {
	vector function;
	char name[NAME_MAX];
} command;

typedef struct cmdlist cmdlist;
struct cmdlist {
	cmdlist * next;
	unsigned int len;
	command cmd[CMDBLK];
};

int cmd_exec (char *, cmdlist *, environment *);
void parse_ln (char *, int *, char **);
vector cmd_exists(const char *, cmdlist *);

#endif /* COMMAND_H */
