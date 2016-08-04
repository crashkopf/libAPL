CC = avr-gcc
AR = avr-ar
ARCH_FLAGS=-mmcu=$(CPU) -DF_CPU=$(SYSCLOCK)L