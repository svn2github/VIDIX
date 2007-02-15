include ../config.mak

NAME = libvidix

LIBNAME_SHARED         = $(NAME).so
LIBNAME_SHARED_MAJOR   = $(NAME).so.$(VIDIX_VERSION_MAJOR)
LIBNAME_SHARED_VERSION = $(NAME).so.$(VIDIX_VERSION)
LIBNAME_STATIC         = $(NAME).a
LIBNAME_WIN32          = $(NAME).dll

LIBDIR = $(prefix)/lib
INCDIR = $(prefix)/include/vidix

SRCS        = vidixlib.c
OBJS_SHARED = vidixlib.o
OBJS_STATIC = vidixlib_static.o

CFLAGS  = $(OPTFLAGS) -I.. -fPIC

ifeq ($(TARGET_OS),OpenBSD)
LDCONFIG_FLAGS += -R
endif

.SUFFIXES: .c .o

# .PHONY: all clean

LIB_TARGETS=
ifeq ($(VIDIX_BUILD_SHARED),yes)
LIB_TARGETS += $(LIBNAME_SHARED_VERSION)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
LIB_TARGETS += $(LIBNAME_STATIC)
endif

all:	$(LIB_TARGETS) do_drivers

$(LIBNAME_SHARED_VERSION): $(SRCS)
	$(CC) -c $(CFLAGS) -o $(OBJS_SHARED) $<
	$(CC) -shared -Wl,-soname -Wl,$@ -o $@ $(OBJS_SHARED) -lc $(LIBDL)
ifneq  ($(TARGET_OS),CYGWIN)
	ln -sf $@ $(LIBNAME_SHARED)
	ln -sf $@ $(LIBNAME_SHARED_MAJOR)
endif

$(LIBNAME_STATIC): $(SRCS)
	$(CC) -c $(CFLAGS) -DVIDIX_BUILD_STATIC -o $(OBJS_STATIC) $<
	$(AR) r $@ $(OBJS_STATIC)

do_drivers:
	$(MAKE) -C drivers

clean:
	rm -f *.a *.o *.so *.so.* *~
	$(MAKE) -C drivers clean

distclean: clean
	rm -f .depend
	$(MAKE) -C drivers distclean

dep depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend
	$(MAKE) -C drivers depend

install:
	mkdir -p $(LIBDIR)
ifeq ($(VIDIX_BUILD_SHARED),yes)
	install -m 755 -s -p $(LIBNAME_SHARED_VERSION) $(LIBDIR)
	ln -sf $(LIBNAME_SHARED_VERSION) $(prefix)/lib/$(LIBNAME_SHARED)
	ln -sf $(LIBNAME_SHARED_VERSION) $(prefix)/lib/$(LIBNAME_SHARED_MAJOR)
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
	cp -f $(LIBNAME_STATIC) $(prefix)/lib
endif

	mkdir -p $(INCDIR)
	install -m 644 fourcc.h $(INCDIR)
	install -m 644 vidix.h $(INCDIR)
	install -m 644 vidixlib.h $(INCDIR)

	$(MAKE) -C drivers install

uninstall:
ifeq ($(VIDIX_BUILD_SHARED),yes)
	rm -f $(LIBDIR)/$(LIBNAME_SHARED)*
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
	rm -f $(LIBDIR)/$(LIBNAME_STATIC)
endif
	rm -f $(INCDIR)/fourcc.h $(INCDIR)/vidix.h $(INCDIR)/vidixlib.h
	$(MAKE) -C drivers uninstall

ifneq ($(wildcard .depend),)
include .depend
endif
