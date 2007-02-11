include ../config.mak

NAME = libdha

MAJOR_VERSION = 0
MINOR_VERSION = 4
VERSION = $(MAJOR_VERSION).$(MINOR_VERSION) 

ifeq ($(TARGET_OS),CYGWIN)
SHORTNAME = $(_NAME).dll
else
SHORTNAME = $(NAME).so.$(MAJOR_VERSION)
SONAME_FLAGS = -Wl,-soname,$(SHORTNAME)
VSHORTNAME = $(NAME).so
endif
ifeq ($(TARGET_WIN32),yes)
LIBNAME = $(NAME).a
SHORTNAME = $(NAME).a
else
LIBNAME = $(NAME).so.$(VERSION)
endif

SRCS = libdha.c \
       mtrr.c \
       pci.c \
       pci_names.c \
       mmi.c \
       ports.c \
       irq.c \
       cpu_flush.c \

OBJS = $(SRCS:.c=.o)

CFLAGS = -fPIC -I. -I.. -Wall -W $(OPTFLAGS)
ifneq ($(TARGET_OS),CYGWIN)
CFLAGS += -fPIC
endif

ifeq ($(TARGET_OS),OpenBSD)
ifeq ($(TARGET_ARCH_X86),yes)
LIBS += -li386
endif
endif

.SUFFIXES: .c .o

# .PHONY: all clean

.c.o: pci_names.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(LIBNAME): $(OBJS)
ifeq ($(TARGET_WIN32),yes)
	$(AR) r $(LIBNAME) $(OBJS)
else
	$(CC) -shared -Wl,-soname -Wl,$(LIBNAME) -o $(LIBNAME) $(OBJS) $(LIBS)
ifneq  ($(TARGET_OS),CYGWIN)
	ln -sf $(LIBNAME) $(SHORTNAME)
	ln -sf $(LIBNAME) $(VSHORTNAME)
endif
endif

all:    $(LIBNAME) $(SHORTNAME)

pci_names.c: oth/pci.db
	LC_ALL=C $(AWK) -f pci_db2c.awk $<

test:
	$(CC) test.c -o test $(SHORTNAME)

clean:
	rm -f *.o *.a *.so.* *.so *~
	rm -f pci_*.c pci_*.h pci.db

distclean: clean
	rm -f .depend test

dep depend: pci_names.c
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

install:
	-mkdir -p $(prefix)/lib
	install -D -m 755 -s -p $(LIBNAME) $(prefix)/lib/$(LIBNAME)
	rm -f $(prefix)/lib/$(NAME).so
	ln -sf $(LIBNAME) $(prefix)/lib/$(SHORTNAME)
ifeq ($(TARGET_OS),OpenBSD)
	-$(LDCONFIG) -R
else
	-$(LDCONFIG)
endif

uninstall:
	rm -f $(prefix)/lib/$(LIBNAME) $(prefix)/lib/$(SHORTNAME)
	rm -f $(prefix)/lib/$(NAME).so
	rmdir -p --ignore-fail-on-non-empty $(prefix)/lib
ifeq ($(TARGET_OS),OpenBSD)
	-$(LDCONFIG) -R
else
	-$(LDCONFIG)
endif

ifneq ($(wildcard .depend),)
include .depend
endif
