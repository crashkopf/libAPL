/*
  btest.c - buffer (FIFO) unit test
*/

#include <stdio.h>
#include <string.h>
#include "buffer.h"

char t[BSIZE]; // Same size as buffer
buffer b = {0, 0, BSIZE, {}};

int nprint (char * c, int n) {
	while (n--) putc(*c++, stdout);
	return 0;
}

int main (int argc, char * argv[]) {

	bflush(&b);
	memset(b.data, '\0', BSIZE);
	memset(t, '\0', sizeof(t));

	// Write 4, read 4
	writeb(&b, "abcd", 4);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	readb(&b, t, 4);
	printf("READ %d:%d %s\n", b.pos, b.len, t);

	bflush(&b);
	memset(b.data, '\0', BSIZE);
	memset(t, '\0', sizeof(t));

	// Write 2, write 2, read 4
	writeb(&b, "ef", 2);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	writeb(&b, "gh", 2);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	readb(&b, t, 4);
	printf("READ %d:%d %s\n", b.pos, b.len, t);
	
	bflush(&b);
	memset(b.data, '\0', BSIZE);
	memset(t, '\0', sizeof(t));

	// Write 4, read 2, read 2
	writeb(&b, "ijkl", 4);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	readb(&b, t, 2);
	printf("READ %d:%d %s\n", b.pos, b.len, t);
	readb(&b, t, 2);
	printf("READ %d:%d %s\n", b.pos, b.len, t);

	bflush(&b);
	memset(b.data, '\0', BSIZE);
	memset(t, '\0', sizeof(t));
	
	writeb(&b, "mnop", 4);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	readb(&b, t, 2);
	printf("READ %d:%d %s\n", b.pos, b.len, t);
	writeb(&b, "qrst", 4);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	readb(&b, t, 6);
	printf("READ %d:%d %s\n", b.pos, b.len, t);
	
	bflush(&b);
	memset(b.data, '\0', BSIZE);
	memset(t, '\0', sizeof(t));
	
	writeb(&b, "uvwxyz", 6);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	readb(&b, t, 2);
	printf("READ %d:%d %s\n", b.pos, b.len, t);
	writeb(&b, "ABCD", 4);
	printf("WRITE %d:%d %s\n", b.pos, b.len, b.data);
	readb(&b, t, 8);
	printf("READ %d:%d %s\n", b.pos, b.len, t);
}
