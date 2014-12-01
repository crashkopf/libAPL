#include "timer.h"
#include "device.h"

typedef struct {
	unsigned int state; 
	unsigned int pri; // process priority
	timer_s timer;
	device *devs[]; // NULL-terminated array of device pointers this process has open
} proc;