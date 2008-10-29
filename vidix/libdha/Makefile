include ../config.mak

NAME = libdha

LIBNAME_SHARED         = $(NAME).so
LIBNAME_SHARED_MAJOR   = $(NAME).so.$(VIDIX_VERSION_MAJOR)
LIBNAME_SHARED_VERSION = $(NAME).so.$(VIDIX_VERSION)
LIBNAME_STATIC         = $(NAME).a
LIBNAME_WIN32          = $(NAME).dll

SRCS = libdha.c \
       mtrr.c \
       pci.c \
       pci_names.c \
       mmi.c \
       ports.c \
       irq.c \
       cpu_flush.c \

OBJS = $(SRCS:.c=.o)

CFLAGS = -fPIC -I. -I.. $(OPTFLAGS)
ifneq ($(TARGET_OS),CYGWIN)
CFLAGS += -fPIC
endif

ifeq ($(TARGET_OS),OpenBSD)
LDCONFIG_FLAGS += -R
ifeq ($(TARGET_ARCH_X86),yes)
LIBS += -li386
endif
endif

.SUFFIXES: .c .o

# .PHONY: all clean

.c.o: pci_names.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(LIBNAME_SHARED_VERSION): $(OBJS)
	$(CC) -shared -Wl,-soname -Wl,$@ -o $@ $(OBJS) $(LIBS)
ifneq  ($(TARGET_OS),CYGWIN)
	ln -sf $@ $(LIBNAME_SHARED)
	ln -sf $@ $(LIBNAME_SHARED_MAJOR)
endif

$(LIBNAME_STATIC): $(OBJS)
	$(AR) r $@ $(OBJS)

$(LIBNAME_WIN32): $(OBJS)
	$(CC) -shared -o $(LIBNAME_WIN32) $(OBJS) $(LIBS)

LIB_TARGETS=
ifeq ($(VIDIX_BUILD_SHARED),yes)
LIB_TARGETS += $(LIBNAME_SHARED_VERSION)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
LIB_TARGETS += $(LIBNAME_STATIC)
endif
ifeq ($(TARGET_WIN32),yes)
LIB_TARGETS += $(LIBNAME_WIN32)
endif

all: $(LIB_TARGETS)

pci_names.c: oth/pci.ids
	LC_ALL=C $(AWK) -f pci_db2c.awk $<

test: pci_names.c $(LIBNAME) $(SHORTNAME)
	$(CC) test.c -o test $(SHORTNAME) -L. -ldha

clean:
	rm -f *.o *.a *.so.* *.so *~
	rm -f pci_*.c pci_*.h pci.db
	rm -f test

distclean: clean
	rm -f .depend

dep depend: pci_names.c
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

install:
	-mkdir -p $(prefix)/lib
ifeq ($(VIDIX_BUILD_SHARED),yes)
	install -D -m 755 -s -p $(LIBNAME_SHARED_VERSION) $(prefix)/lib/$(LIBNAME_SHARED_VERSION)
	ln -sf $(LIBNAME_SHARED_VERSION) $(prefix)/lib/$(LIBNAME_SHARED)
	ln -sf $(LIBNAME_SHARED_VERSION) $(prefix)/lib/$(LIBNAME_SHARED_MAJOR)
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
	cp -f $(LIBNAME_STATIC) $(prefix)/lib/$(LIBNAME_STATIC)
endif

uninstall:
ifeq ($(VIDIX_BUILD_SHARED),yes)
	rm -f $(prefix)/lib/$(LIBNAME_SHARED)*
	-$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_BUILD_STATIC),yes)
	rm -f $(prefix)/lib/$(LIBNAME_STATIC)
endif

ifneq ($(wildcard .depend),)
include .depend
endif
