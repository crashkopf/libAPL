/*
  cmdtest.c
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <shell/shell.h>

int main(void){

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
