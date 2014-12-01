CPU ?= atmega1280
SYSCLOCK ?= 16000000
INCLUDES = -I/usr/lib/avr/include -iquote "$(CURDIR)/include"
CC = avr-gcc
CFLAGS = -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=$(CPU) -DF_CPU=$(SYSCLOCK)L $(INCLUDES)
AR = avr-ar

drivers := driver/serio.o driver/adc.o driver/adcmap.o driver/hd44780.o
shell := shell/shell.o shell/command.o shell/debug.o shell/adcutils.o
util := buffer.o event.o tree.o cron.o
sys := sys/device.o sys/proc.o
ui := ui/view.o ui/viewmux.o ui/menu.o ui/dialog.o

objs := $(drivers) $(shell) $(util) $(ui)

library := libAPL.a

%.o: %.c include/%.h
	$(CC) $(CFLAGS) -o $@ $<

libAPL.a: $(objs)
	$(AR) rcs libAPL.a $(objs)

all: libAPL.a
	
clean:
	rm -f $(library) $(objs)

.PHONY: all install clean test
