/*
  iodev.h - Sequential and random access device abstraction layer
*/

typedef struct {
	int (*)() read,
	int (*)() write,
} iodev;