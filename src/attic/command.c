/*
  command.c - embedded command line system
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell/command.h"

#ifndef __AVR__
#include "strsep.c"
#endif


/*
  Search for a command in the list.  Returns a vector on succes or NULL on failure.
*/
vector cmd_exists(const char * name, cmdlist * list) {
	unsigned int x;
	while (list) {
		for(x=0; x < list->len; x++) {
			if (list->cmd[x].name && strcmp(name, list->cmd[x].name) == 0) {
				return list->cmd[x].function;
			}
		}
		list = list->next;
	}
	return 0;
}

int cmd_add(const char * name, vector function, cmdlist * list) {
	if (!list) return 0; // List is NULL
	if (cmd_exists(name, list)) return 0; // That command already exists.  Remove it first.
	while (list) {
		if (list->len < CMDBLK) {
			list->cmd[list->len].function = function;
			strncpy(list->cmd[list->len].name, name, NAME_MAX);
			list->len++;
			return 1;
		}
		else if (!list->next) list->next = malloc(sizeof(list)); 
		list = list->next;
	}
	return 0;
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
int cmd_exec  (char * str, cmdlist * list, environment * env) {
	vector cmd;
	int argc;
	char * argv[ARGC_MAX];
	
	parse_ln(str, &argc, argv);
	
	if (argc) {
		 if((cmd = cmd_exists(argv[0], list))) {
			return (*cmd)(argc, argv, env);
		}
	}
	return ~0;
}



