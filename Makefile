#PROJECT ?=

SRCDIR ?= src
BUILDDIR ?= build
INCLUDEDIR ?= include

TARGET ?=
CPU ?=
ARCH ?=

-include $(SRCDIR)/$(TARGET)/profile.mk
-include $(SRCDIR)/$(CPU)/profile.mk
-include $(SRCDIR)/$(ARCH)/profile.mk

INCLUDES = $(INCLUDEDIR) $(INCLUDEDIR)/$(TARGET) $(INCLUDEDIR)/$(CPU) $(INCLUDEDIR)/$(ARCH)

DEFINES = -D_ARCH=$(ARCH) -D_CPU=$(CPU) -D_TARGET=$(TARGET) -D_PROJECT=$(PROJECT)

CFLAGS = -c -Os -Wall -Wno-main -fno-exceptions -ffunction-sections -fdata-sections $(DEFINES) $(addprefix -iquote , $(INCLUDES)) $(ARCH_FLAGS) $(HWDEFS)

#.IGNORE $(addsuffix /serio.h, $(INCLUDES))

VPATH = $(SRCDIR)

library := libcrashkopf.a

all: serio.o buffer.o

serio.o: $(CPU)/serio.c $(addsuffix /serio.h, $(INCLUDES)) buffer.o
buffer.o: buffer.c $(INCLUDEDIR)/buffer.h

#$(objs): $(BUILDDIR)/%.o: %.c
#	mkdir -p $(@D)
#	$(CC) $(CFLAGS) -MD -o $@ $<

#$(library): $(DEVICE) $(objs)
#
#$(AR) rcs $@ $^

#-include $(objs:.o=.d)

%.o:
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(library) $(objs) $(objs:.o=.d) 

.PHONY: all install clean test

