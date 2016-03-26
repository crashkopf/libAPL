DEVICE ?=
ARCH ?=

-include devices/$(DEVICE)/profile.mk
-include arch/$(ARCH)/profile.mk

INCLUDES = -I/usr/lib/avr/include  -iquote "$(CURDIR)/include/$(DEVICE)" -iquote "$(CURDIR)/include/$(ARCH)" -iquote "$(CURDIR)/include"

CFLAGS = -c -Os -Wall -Wno-main -fno-exceptions -ffunction-sections -fdata-sections  $(INCLUDES) $(ARCH_FLAGS) $(HWDEFS)

dirs := \
	arch/$(ARCH) \
	devices/$(DEVICE) \
	utils \
	sys \
#	shell \
#	ui

srcs := $(foreach I,$(dirs),$(wildcard $I/*.c))
objs := $(srcs:%.c=%.o)
library := libcrashkopf.a

all: $(library)

$(library): $(objs)
	$(AR) rcs $@ $^

-include $(objs:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -MD -o $@ $<

clean:
	rm -f $(library) $(objs) $(objs:.o=.d)

.PHONY: all install clean test

