include ../config.mak

NAME = libvidix

LIBNAME_SHARED         = $(NAME).so
LIBNAME_SHARED_MAJOR   = $(NAME).so.$(VIDIX_VERSION_MAJOR)
LIBNAME_SHARED_VERSION = $(NAME).so.$(VIDIX_VERSION)
LIBNAME_WIN32          = $(NAME).dll
LIBNAME                = $(NAME).a

INCDIR = $(prefix)/include/vidix

SRCS        = vidixlib.c

OBJS=$(SRCS:.c=.o)
SHOBJS=$(SRCS:.c=.po)

CFLAGS  = $(OPTFLAGS) -I..
SHCFLAGS= $(CFLAGS) -fPIC

SUBDIRS = drivers
DO_MAKE = @ for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit; done
DO_ALL  = @ for i in $(SUBDIRS); do $(MAKE) -C $$i all || exit; done

ifeq ($(TARGET_OS),OpenBSD)
LDCONFIG_FLAGS += -R
endif

.SUFFIXES: .c .o .po

ifeq ($(VIDIX_BUILD_STATIC),yes)
.PHONY: $(SUBDIRS)
endif

LIB_TARGETS=
ifeq ($(VIDIX_BUILD_SHARED),yes)
LIB_TARGETS += $(LIBNAME_SHARED_VERSION)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
LIB_TARGETS += $(LIBNAME)
endif
ifeq ($(TARGET_WIN32),yes)
LIB_TARGETS += $(LIBNAME_WIN32)
endif

ifeq ($(VIDIX_BUILD_STATIC),yes)
all:	$(LIB_TARGETS)
endif
ifeq ($(VIDIX_BUILD_SHARED),yes)
all:	$(LIB_TARGETS) do_drivers
endif

ifeq ($(VIDIX_BUILD_STATIC),yes)
$(SUBDIRS):
	$(DO_ALL)
endif

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.c.po:
	$(CC) -c $(SHCFLAGS) -o $@ $<

ifeq ($(VIDIX_BUILD_SHARED),yes)
$(LIBNAME_SHARED_VERSION): $(SHOBJS)
	$(CC) $(LDFLAGS) -shared -Wl,-soname -Wl,$@ -o $@ $(SHOBJS) -lc $(LIBDL)
$(LIBNAME_WIN32): $(OBJS)
	$(CC) -shared -o $(LIBNAME_WIN32) $(SHOBJS) $(LIBS)
ifneq  ($(TARGET_OS),CYGWIN)
	ln -sf $@ $(LIBNAME_SHARED)
	ln -sf $@ $(LIBNAME_SHARED_MAJOR)
endif
endif

ifeq ($(VIDIX_BUILD_STATIC),yes)
$(LIBNAME):  $(SUBDIRS) $(OBJS)
	$(AR) r $@ $(OBJS) \
	$(wildcard drivers/*.o)
endif

do_drivers:
	$(MAKE) -C drivers

clean:
	rm -f *.a *.o *.po *.so *.so.* *~
	$(MAKE) -C drivers clean

distclean: clean
	rm -f .depend
	$(MAKE) -C drivers distclean

dep depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend
	$(MAKE) -C drivers depend

install:
	mkdir -p $(DESTDIR)$(LIBDIR)
ifeq ($(VIDIX_BUILD_SHARED),yes)
	install -m 755 -s -p $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)
	ln -sf $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED)
	ln -sf $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED_MAJOR)
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
	cp -f $(LIBNAME) $(DESTDIR)$(LIBDIR)
endif

	mkdir -p $(DESTDIR)$(INCDIR)
	install -m 644 fourcc.h $(DESTDIR)$(INCDIR)
	install -m 644 vidix.h $(DESTDIR)$(INCDIR)
	install -m 644 vidixlib.h $(DESTDIR)$(INCDIR)

	$(MAKE) -C drivers install

uninstall:
ifeq ($(VIDIX_BUILD_SHARED),yes)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED)*
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME)
endif
	rm -f $(DESTDIR)$(INCDIR)/fourcc.h $(DESTDIR)$(INCDIR)/vidix.h $(DESTDIR)$(INCDIR)/vidixlib.h
	$(MAKE) -C drivers uninstall

ifneq ($(wildcard .depend),)
include .depend
endif
