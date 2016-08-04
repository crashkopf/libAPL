#PROJECT ?=

SRCDIR ?= src
BUILDDIR ?= build
INCLUDEDIR ?= include

TARGET ?=
CPU ?=
ARCH ?=

-include src/$(TARGET)/profile.mk
-include src/$(CPU)/profile.mk
-include src/$(ARCH)/profile.mk

INCLUDES = $(INCLUDEDIR) $(INCLUDEDIR)/$(TARGET) $(INCLUDEDIR)/$(CPU) $(INCLUDEDIR)/$(ARCH)

DEFINES = -D _ARCH=$(ARCH) -D _CPU=$(CPU) -D _TARGET=$(TARGET) -D _PROJECT=$(PROJECT)

CFLAGS = -E -c -Os -Wall -Wno-main -fno-exceptions -ffunction-sections -fdata-sections $(DEFINES) $(addprefix -iquote , $(INCLUDES)) $(ARCH_FLAGS) $(HWDEFS)

#.IGNORE $(addsuffix /serio.h, $(INCLUDES))

serio.o: src/$(CPU)/serio.c $(addsuffix /serio.h, $(INCLUDES))

#srcs += $(foreach I,$(dirs),$(wildcard $I/*.c))
#objs := $(srcs:%.c=$(BUILDDIR)/%.o)
#library := libcrashkopf.a

#all: $(library)

#serio.o: src/$(DEVICE)/serio.c $(IDIR)/serio.h $(IDIR)/$(ARCH)/serio.h $(IDIR)/$(DEVICE)/serio.h


#$(DEVICE):
#	-include ./device/$(@)/profile.mk

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

