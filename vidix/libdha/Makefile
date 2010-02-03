include ../config.mak

NAME = libdha

LIBNAME_SHARED         = $(NAME).so
LIBNAME_SHARED_MAJOR   = $(NAME).so.$(VIDIX_VERSION_MAJOR)
LIBNAME_SHARED_VERSION = $(NAME).so.$(VIDIX_VERSION)
LIBNAME_WIN32          = $(NAME).dll
LIBNAME                = $(NAME).a

SRCS = libdha.c \
       mtrr.c \
       pci.c \
       pci_names.c \
       mmi.c \
       ports.c \
       irq.c \
       cpu_flush.c \

OBJS = $(SRCS:.c=.o)
SHOBJS = $(SRCS:.c=.po)

CFLAGS = -DVIDIX_COMPILE_STATIC -I. -I.. $(OPTFLAGS)
SHCFLAGS = -fPIC -I. -I.. $(OPTFLAGS)

ifeq ($(TARGET_OS),OpenBSD)
LDCONFIG_FLAGS += -R
ifeq ($(TARGET_ARCH_X86),yes)
LIBS += -li386
endif
endif

.SUFFIXES: .c .o .po

LIB_TARGETS=
ifeq ($(VIDIX_SHARED),yes)
LIB_TARGETS += $(LIBNAME_SHARED_VERSION)
endif
ifeq ($(VIDIX_STATIC),yes)
LIB_TARGETS += $(LIBNAME)
endif
ifeq ($(TARGET_WIN32),yes)
LIB_TARGETS += $(LIBNAME_WIN32)
endif

all: $(LIB_TARGETS)

.c.o: pci_names.c
	$(CC) -c $(CFLAGS) -o $@ $<

.c.po: pci_names.c
	$(CC) -c $(SHCFLAGS) -o $@ $<

$(LIBNAME_SHARED_VERSION): $(SHOBJS)
	$(CC) $(LDFLAGS) -shared -Wl,-soname -Wl,$@ -o $@ $(SHOBJS) $(LIBS)
ifneq  ($(TARGET_OS),CYGWIN)
	ln -sf $@ $(LIBNAME_SHARED)
	ln -sf $@ $(LIBNAME_SHARED_MAJOR)
endif

$(LIBNAME): $(OBJS)
	$(AR) r $@ $(OBJS)

$(LIBNAME_WIN32): $(SHOBJS)
	$(CC) $(LDFLAGS) -shared -o $(LIBNAME_WIN32) $(SHOBJS) $(LIBS)


pci_names.c: oth/pci.ids
	LC_ALL=C $(AWK) -f pci_db2c.awk $<

test: pci_names.c $(LIBNAME) $(SHORTNAME)
	$(CC) test.c -o test $(SHORTNAME) -L. -ldha

clean:
	rm -f *.o *.a *.po *.so.* *.so *~
	rm -f pci_*.c pci_*.h pci.db
	rm -f test

distclean: clean
	rm -f .depend

dep depend: pci_names.c
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

install:
	-mkdir -p $(DESTDIR)$(LIBDIR)
ifeq ($(VIDIX_SHARED),yes)
	install -D -m 755 -s -p $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED_VERSION)
	ln -sf $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED)
	ln -sf $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED_MAJOR)
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_STATIC),yes)
	cp -f $(LIBNAME) $(DESTDIR)$(LIBDIR)/$(LIBNAME)
endif

uninstall:
ifeq ($(VIDIX_SHARED),yes)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED)*
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_STATIC),yes)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME_STATIC)
endif

ifneq ($(wildcard .depend),)
include .depend
endif
