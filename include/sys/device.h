
#ifndef _BUFFER_H
#include <buffer.h>
#endif

typedef struct device {
	int (*init)(struct device *);
	int (*open)(struct device *);
	int (*read)(struct device *, void *, unsigned int);
	int (*write)(struct device *, void *, unsigned int);
	unsigned int block;  // Block size in bytes
	buffer *buf;  // Buffer structure pointer
	uint8_t	flags;
} device;

