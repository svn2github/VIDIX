include ../config.mak

ifeq ($(TARGET_OS),CYGWIN)
LIBNAME = libvidix.dll
else
LIBNAME = libvidix.so
endif
BINDIR = $(prefix)/lib
INCDIR = $(prefix)/include/vidix
SUBDIRS = drivers
DO_MAKE = @ for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit; done


SRCS    = vidixlib.c
OBJS	= $(SRCS:.c=.o)

CFLAGS  = $(OPTFLAGS) -W -Wall -fPIC

.SUFFIXES: .c .o

# .PHONY: all clean

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all:	$(SUBDIRS) $(LIBNAME)
	$(DO_MAKE)

$(LIBNAME):	$(OBJS)
	$(LD) -shared -soname $(LIBNAME) -o $(LIBNAME) $(OBJS) -lc $(LIBDL)

clean:
	rm -f *.o $(LIBNAME) *~
	$(DO_MAKE)

distclean:
	rm -f test *.o $(LIBNAME) *~ .depend
	$(DO_MAKE)

dep:    depend
	$(DO_MAKE)

depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

install:
	$(DO_MAKE)
	mkdir -p $(BINDIR)
	install -m 755 -s -p $(LIBNAME) $(BINDIR)
ifeq ($(TARGET_OS),OpenBSD)
	$(LDCONFIG) -R
else
	$(LDCONFIG)
endif
	mkdir -p $(INCDIR)
	install -m 644 fourcc.h $(INCDIR)/fourcc.h
	install -m 644 vidix.h $(INCDIR)/vidix.h
	install -m 644 vidixlib.h $(INCDIR)/vidixlib.h

uninstall:
	$(DO_MAKE)
	rm -f $(BINDIR)/$(LIBNAME)
	rmdir -p --ignore-fail-on-non-empty $(BINDIR)
	rm -f $(INCDIR)/*
	rmdir -p --ignore-fail-on-non-empty $(INCDIR)

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
