/*
  command.c - embedded command line system
*/

#include <stdio.h>
#include <string.h>
#include "shell/command.h"

#ifndef __AVR__
#include "fakeavr.h"
#endif

/*
  Search for a command in the map.  Returns a vector on succes or NULL on failure.
*/
vector cmd_exists(char * name, command clist[]) {
	unsigned int x = 0;
	if (clist != NULL) {
		while (clist[x].name != NULL) {
			if (clist[x].name && strcmp_P(name, clist[x].name) == 0) {
				return clist[x].function;
			}
			x++;
		}
	}
	return NULL;
}

/*
 parse_ln - takes a command string and creates pointers to each argument
 str - a pointer to the command string.
 argc - a pointer to the number of arguments
 argv - a pointer to an array of pointers (argument vectors)
 No return value. This function modifies all of its arguments.  Suck it.
 Example: 
 char * buf[];
 int argc;
 char * argv[];
 parse_ln(&buf, &argc, &argv);
*/
void parse_ln (char * str, int * argc, char * argv[]) {
	*argc = 0;
	if (str) {
		while ((argv[*argc] = strsep(&str, " \n")) && *argc < ARGC_MAX) {
			if (strlen(argv[*argc])) {
				*argc += 1;
			}
		}
	}
}

/*
  cmd_exec - Call a command, but only if it exists in the command map
*/
int cmd_exec  (char * str, command clist[], environment * env) {
	vector cmd;
	int argc;
	char * argv[ARGC_MAX];
	
	parse_ln(str, &argc, argv);
	
	if (argc && (argv != NULL)) {
		 if((cmd = cmd_exists(argv[0], clist)) != NULL) {
			return (*cmd)(argc, argv, env);
		}
	}
	return ~0;
}
