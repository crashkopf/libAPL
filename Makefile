#PROJECT ?=

LIBCRASHKOPF ?= .
SRCDIR ?= $(LIBCRASHKOPF)/src
INCLUDEDIR ?= $(LIBCRASHKOPF)/include

INCLUDES = $(INCLUDEDIR)

VPATH = $(SRCDIR)

CFLAGS = -c -Os -Wall -Wno-main

ifdef TARGET
include $(SRCDIR)/$(TARGET)/profile.mk
INCLUDES += $(INCLUDEDIR)/$(TARGET)
DEFINES += _ARCH=$(ARCH)
VPATH += $(SRCDIR)/$(TARGET)
endif

ifdef CPU
include $(SRCDIR)/$(CPU)/profile.mk
INCLUDES += $(INCLUDEDIR)/$(CPU)
DEFINES += _CPU=$(CPU)
VPATH += $(SRCDIR)/$(CPU)
endif

ifdef ARCH
include $(SRCDIR)/$(ARCH)/profile.mk
INCLUDES += $(INCLUDEDIR)/$(ARCH)
DEFINES += _TARGET=$(TARGET)
VPATH += $(SRCDIR)/$(ARCH)
endif

#CFLAGS = -c -Os -Wall -Wno-main -fno-exceptions -ffunction-sections -fdata-sections $(addprefix -D, $(DEFINES)) $(addprefix -I, $(INCLUDES)) $(ARCH_FLAGS) $(HWDEFS)
CFLAGS += $(addprefix -D, $(DEFINES)) $(addprefix -I, $(INCLUDES)) $(ARCH_FLAGS) $(HWDEFS)

all: serio.o buffer.o

serio.o: serio.c $(addsuffix /serio.h, $(INCLUDES)) buffer.o
buffer.o: buffer.c $(INCLUDEDIR)/buffer.h

#-include $(objs:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(library) $(objs) $(objs:.o=.d) 

.PHONY: all install clean test

