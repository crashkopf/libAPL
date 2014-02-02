INCLUDES = -I/usr/lib/avr/include
OBJS = command.o shell.o
CPU = atmega1280
SYSCLOCK = 16000000

CC = avr-gcc
CFLAGS = -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=$(CPU) -DF_CPU=$(SYSCLOCK)L $(INCLUDES)

ISP = arduino
PORTDEV = /dev/ttyUSB0
PORTSPEED = 57600

%.o: %.c *.h
	$(CC) $(CFLAGS) -o $@ $<

#project.elf: $(OBJS)
#	$(CC) -mmcu=$(CPU) -o project.elf $^
	
#project.hex: project.elf
#	avr-objcopy -j .text -j .data -O ihex $< $@

#install: project.hex
#	avrdude -p$(CPU) -c$(ISP) -P$(PORTDEV) -b$(PORTSPEED) -D -Uflash:w:project.hex:i
	
clean:
	rm $(OBJS)

