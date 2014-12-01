CPU ?= atmega1280
SYSCLOCK ?= 16000000
INCLUDES = -I/usr/lib/avr/include -iquote "$(CURDIR)/include"
CC = avr-gcc
CFLAGS = -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=$(CPU) -DF_CPU=$(SYSCLOCK)L $(INCLUDES)
AR = avr-ar

dirs := include utils driver sys shell ui
srcs := $(foreach I,$(dirs),$(wildcard $I/*.c)) main.c
objs := $(srcs:%.c=%.o)
library := libAPL.a

all: libAPL.a

libAPL.a: $(objs)
	$(AR) rcs libAPL.a $(objs)

-include $(objs:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -MD -o $@ $<

clean:
	rm -f $(library) $(objs) $(objs:.o=.d)

.PHONY: all install clean test

