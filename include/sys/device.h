
#ifndef _DEVICE_H
#include "util/buffer.h"

typedef struct device {
	int (*init)(struct device *);
	int (*open)(struct device *);
	int (*read)(struct device *, void *, unsigned int);
	int (*write)(struct device *, void *, unsigned int);
	unsigned int block;  // Block size in bytes
	buffer *buf;  // Buffer structure pointer
	uint8_t	flags;
} device;

#define _DEVICE_H
#endif