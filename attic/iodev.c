/*
  iodev.h - Sequential and random access device abstraction layer
  This library provides a consistant interface for managing I/O devices.
  All I/O devices must provide the following facilities:
	* A function to read and/or write an arbitraily-sized block of data to the device - read() /write()
	* A function to determine if the device will block a write operation 
	* A function to determine if the device will block a read operation
  
*/

#include "iodev.h"