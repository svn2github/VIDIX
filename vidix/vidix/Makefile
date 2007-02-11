include ../config.mak

NAME = libvidix

MAJOR_VERSION = 0
MINOR_VERSION = 9
MICRO_VERSION = 9.3
VERSION = $(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)

ifeq ($(TARGET_OS),CYGWIN)
SHORTNAME = $(NAME).dll
LIBNAME = $(NAME).dll
else
SHORTNAME = $(NAME).so.$(MAJOR_VERSION)
VSHORTNAME = $(NAME).so
LIBNAME = $(NAME).so.$(VERSION)
endif

LIBDIR = $(prefix)/lib
INCDIR = $(prefix)/include/vidix

SRCS    = vidixlib.c
OBJS	= $(SRCS:.c=.o)

CFLAGS  = $(OPTFLAGS) -W -Wall -fPIC

.SUFFIXES: .c .o

# .PHONY: all clean

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all:	$(LIBNAME) do_drivers

do_drivers:
	$(MAKE) -C drivers

$(LIBNAME): $(OBJS)
	$(CC) -shared -Wl,-soname -Wl,$(LIBNAME) \
	   -o $(LIBNAME) $(OBJS) -lc $(LIBDL)
ifneq  ($(TARGET_OS),CYGWIN)
	ln -sf $(LIBNAME) $(SHORTNAME)
	ln -sf $(LIBNAME) $(VSHORTNAME)
endif

clean:
	rm -f *.o *.so *.so.* *~
	$(MAKE) -C drivers clean

distclean:
	rm -f *.o *.so *.so.* *~ .depend
	$(MAKE) -C drivers distclean

dep depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend
	$(MAKE) -C drivers depend

install: $(LIBNAME)
	mkdir -p $(LIBDIR)
	install -m 755 -s -p $(LIBNAME) $(LIBDIR)
ifeq ($(TARGET_OS),OpenBSD)
	$(LDCONFIG) -R
else
	$(LDCONFIG)
endif
	mkdir -p $(INCDIR)
	install -m 644 fourcc.h $(INCDIR)/fourcc.h
	install -m 644 vidix.h $(INCDIR)/vidix.h
	install -m 644 vidixlib.h $(INCDIR)/vidixlib.h
	$(MAKE) -C drivers install

uninstall:
	rm -f $(LIBDIR)/$(LIBNAME)
	rmdir -p --ignore-fail-on-non-empty $(LIBDIR)
	rm -f $(INCDIR)/*
	rmdir -p --ignore-fail-on-non-empty $(INCDIR)
	$(MAKE) -C drivers uninstall

ifneq ($(wildcard .depend),)
include .depend
endif
