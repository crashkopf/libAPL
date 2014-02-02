ifndef UNITTEST
  CPU ?= atmega1280
  SYSCLOCK ?= 16000000
  INCLUDES = -I/usr/lib/avr/include -iquote "$(CURDIR)/include"
  CC = avr-gcc
  CFLAGS = -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=$(CPU) -DF_CPU=$(SYSCLOCK)L $(INCLUDES)
  AR = avr-ar
else
  INCLUDES = -iquote "$(CURDIR)/include" -iquote "$(CURDIR)/test/include"
  CC = gcc
  CFLAGS = -c -g -Wall $(INCLUDES)
endif

drivers := driver/serio.o driver/adc.o driver/adcmap.o driver/hd44780.o
shell := shell/shell.o shell/command.o shell/debug.o shell/adcutils.o
util := buffer.o event.o

objs := $(drivers) $(shell) $(util)

library := libAPL.a

%.o: %.c *.h
	$(CC) $(CFLAGS) -o $@ $<

libAPL.a: $(objs)
	$(AR) rcs libAPL.a $(objs)

all: libAPL.a
	
clean:
	rm -f $(library) $(objs)

.PHONY: all install clean test
