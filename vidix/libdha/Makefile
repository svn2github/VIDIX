# makefile

include ../config.mak

VERSION = 0.1

ifeq ($(TARGET_OS),CYGWIN)
SHORTNAME = libdha.dll
else
SHORTNAME = libdha.so
endif
LIBNAME = libdha-$(VERSION).so

SRCS=libdha.c mtrr.c pci.c pci_names.c
OBJS=$(SRCS:.c=.o)

CFLAGS  = $(OPTFLAGS) -fPIC -I. -I.. -Wall -W
LIBS = 
ifeq ($(TARGET_OS),OpenBSD)
LIBS += -li386
endif

.SUFFIXES: .c .o

# .PHONY: all clean

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

$(LIBNAME):     $(OBJS)
	$(CC) -shared -o $(LIBNAME) $(OBJS) $(LIBS)
	ln -sf $(LIBNAME) $(SHORTNAME)

all:    $(LIBNAME) $(SHORTNAME)

pci_names.c:
	$(AWK) -f pci_db2c.awk oth/pci.db

test:
	$(CC) test.c -o test $(SHORTNAME)

clean:
	rm -f *.o *.so *~

distclean:
	rm -f Makefile.bak *.o *.so test *~ .depend
	rm -f pci_dev_ids.c pci_ids.h pci_names.c pci_names.h pci_vendors.h pci.db

dep:    depend

depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

install:
	install -D -m 755 -s -p $(LIBNAME) $(prefix)/lib/$(LIBNAME)
	rm -f $(prefix)/lib/libdha.so
	ln -sf $(LIBNAME) $(prefix)/lib/libdha.so
ifeq ($(TARGET_OS),OpenBSD)
	$(LDCONFIG) -R
else
	$(LDCONFIG)
endif
uninstall:
	rm -f $(prefix)/lib/$(LIBNAME)
	rm -f $(prefix)/lib/libdha.so
	rmdir -p --ignore-fail-on-non-empty $(prefix)/lib
#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif