include ../config.mak

NAME = libvidix

LIBNAME_SHARED         = $(NAME).so
LIBNAME_SHARED_MAJOR   = $(NAME).so.$(VIDIX_VERSION_MAJOR)
LIBNAME_SHARED_VERSION = $(NAME).so.$(VIDIX_VERSION)
LIBNAME_SHAREDXX         = $(NAME)xx.so
LIBNAME_SHAREDXX_MAJOR   = $(NAME)xx.so.$(VIDIX_VERSION_MAJOR)
LIBNAME_SHAREDXX_VERSION = $(NAME)xx.so.$(VIDIX_VERSION)
LIBNAME_WIN32          = $(NAME).dll
LIBNAME                = $(NAME).a

INCDIR = $(prefix)/include/vidix

SRCS        = vidixlib.c
CXXSRCS     = vidixlibxx.cpp
HEADERS     = fourcc.h vidix.h vidix_driver.h vidix_enums.h vidix_types.h vidixlib.h vidixlibxx.h

OBJS=$(SRCS:.c=.o)
CXXOBJS=$(CXXSRCS:.cpp=.o)
SHOBJS=$(SRCS:.c=.po)
CXXSHOBJS=$(CXXSRCS:.cpp=.po)

CFLAGS   = $(OPTFLAGS) -I.. -DVIDIX_COMPILE_STATIC
CXXFLAGS = $(OPTXXFLAGS) -I.. -DVIDIX_COMPILE_STATIC
SHCFLAGS= $(OPTFLAGS) -I.. -fPIC
CXXSHCFLAGS= $(OPTXXFLAGS) -I.. -fPIC -DVIDIX_PATH='"$(LIBDIR)/vidix/"'

SUBDIRS = drivers
DO_MAKE = @ for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit; done
DO_ALL  = @ for i in $(SUBDIRS); do $(MAKE) -C $$i all || exit; done

ifeq ($(TARGET_OS),OpenBSD)
LDCONFIG_FLAGS += -R
endif

.SUFFIXES: .cpp .c .o .po

ifeq ($(VIDIX_STATIC),yes)
.PHONY: $(SUBDIRS)
endif

LIB_TARGETS=
ifeq ($(VIDIX_SHARED),yes)
LIB_TARGETS += $(LIBNAME_SHARED_VERSION) $(LIBNAME_SHAREDXX_VERSION)
endif
ifeq ($(VIDIX_STATIC),yes)
LIB_TARGETS += $(LIBNAME)
endif
ifeq ($(TARGET_WIN32),yes)
LIB_TARGETS += $(LIBNAME_WIN32)
endif

ifeq ($(VIDIX_STATIC),yes)
all:	$(LIB_TARGETS)
endif
ifeq ($(VIDIX_SHARED),yes)
all:	$(LIB_TARGETS) do_drivers
endif

ifeq ($(VIDIX_STATIC),yes)
$(SUBDIRS):
	$(DO_ALL)
endif

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<
.c.po:
	$(CC) -c $(SHCFLAGS) -o $@ $<
.cpp.po:
	$(CXX) -c $(CXXSHCFLAGS) -o $@ $<

ifeq ($(VIDIX_SHARED),yes)
$(LIBNAME_SHARED_VERSION): $(SHOBJS)
	$(CC) $(LDFLAGS) -shared -Wl,-soname -Wl,$@ -o $@ $(SHOBJS) -ldl -lc $(LIBDL)
	ln -sf $@ $(LIBNAME_SHARED_MAJOR)
	ln -sf $(LIBNAME_SHARED_MAJOR) $(LIBNAME_SHARED)
$(LIBNAME_SHAREDXX_VERSION): $(CXXSHOBJS)
	$(CXX) $(LDXXFLAGS) -shared -Wl,-soname -Wl,$@ -o $@ $(CXXSHOBJS) -L./ -lvidix
$(LIBNAME_WIN32): $(OBJS)
	$(CC) -shared -o $(LIBNAME_WIN32) $(SHOBJS) $(LIBS)
ifneq  ($(TARGET_OS),CYGWIN)
	ln -sf $@ $(LIBNAME_SHARED)
	ln -sf $@ $(LIBNAME_SHARED_MAJOR)
endif
endif

ifeq ($(VIDIX_STATIC),yes)
$(LIBNAME):  $(SUBDIRS) $(OBJS) $(CXXOBJS)
	$(AR) r $@ $(OBJS) $(CXXOBJS) \
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
	$(CC) -MM $(CFLAGS) $(SRCS) $(CXXSRCS) 1>.depend
	$(MAKE) -C drivers depend

install:
	mkdir -p $(DESTDIR)$(LIBDIR)
ifeq ($(VIDIX_SHARED),yes)
	install -m 755 -s -p $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)
	install -m 755 -s -p $(LIBNAME_SHAREDXX_VERSION) $(DESTDIR)$(LIBDIR)
	ln -sf $(LIBNAME_SHARED_VERSION) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED_MAJOR)
	ln -sf $(LIBNAME_SHARED_MAJOR) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED)
	ln -sf $(LIBNAME_SHAREDXX_VERSION) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHAREDXX_MAJOR)
	ln -sf $(LIBNAME_SHAREDXX_MAJOR) $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHAREDXX)
	$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_STATIC),yes)
	cp -f $(LIBNAME) $(DESTDIR)$(LIBDIR)
endif

	mkdir -p $(DESTDIR)$(INCDIR)
	for h in $(HEADERS); do \
	install -m 644 $$h $(DESTDIR)$(INCDIR); \
	done;
	$(MAKE) -C drivers install

uninstall:
ifeq ($(VIDIX_SHARED),yes)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHARED)*
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME_SHAREDXX)*
	$(LDCONFIG) $(LDCONFIG_FLAGS)
endif
ifeq ($(VIDIX_STATIC),yes)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME)
endif
	rm -f $(DESTDIR)$(INCDIR)/fourcc.h $(DESTDIR)$(INCDIR)/vidix.h $(DESTDIR)$(INCDIR)/vidixlib.h
	$(MAKE) -C drivers uninstall

ifneq ($(wildcard .depend),)
include .depend
endif
