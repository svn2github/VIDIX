include ../../config.mak

DRV_LIBDIR = $(LIBDIR)/vidix
OLDLIBDIR = $(prefix)/lib/mplayerxp/vidix

VIDIX_LIBS = -L../../libdha -ldha
VIDIX_LIBS_M = $(VIDIX_LIBS) -lm
VIDIX_LIBS_X = $(VIDIX_LIBS_M) $(X_LIB)
CFLAGS = -W -Wall -fPIC -I. -I.. -I../.. $(OPTFLAGS)
SHCFLAGS = -fPIC $(CFLAGS)

SRCS=   cyberblade_vid.c \
	genfb_vid.c \
	mach64_vid.c \
	mga_crtc2_vid.c \
	mga_tv_vid.c \
	mga_vid.c \
	nvidia_vid.c \
	pm2_vid.c \
	pm3_vid.c \
	radeon_vid.c \
	rage128_vid.c \
	radeonhd_vid.c \
	s3_vid.c \
	savage_vid.c \
	sis_vid.c \
	unichrome_vid.c
ifeq ($(TARGET_OS),Linux)
ifeq ($(HAVE_IVTV),yes)
SRCS += ivtv_vid.c
endif
endif

OBJS = $(SRCS:.c=.o)
SHOBJS = $(SRCS:.c=.po)
DRV  = $(SHOBJS:.po=.so)

ifeq ($(VIDIX_BUILD_STATIC),yes)
all: $(OBJS)
endif
ifeq ($(VIDIX_BUILD_SHARED),yes)
all: $(DRV)
endif

.SUFFIXES: .c .o .po

# .PHONY: all clean

# Object files
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.po: %.c
	$(CC) -c $(SHCFLAGS) -o $@ $<

# Dynamic librairies
%.so: %.po
	$(CC) $(LDFLAGS) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

clean:
	rm -f *.o *.po *.so *~

distclean: clean
	rm -f .depend

dep depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

install:
	mkdir -p $(DESTDIR)$(DRV_LIBDIR)
	install -m 755 -p *.so $(DESTDIR)$(DRV_LIBDIR)
	if test -d $(OLDLIBDIR) ; then rm -f $(OLDLIBDIR)/*.so ; rmdir -p --ignore-fail-on-non-empty $(OLDLIBDIR) ; fi

uninstall:
	rm -f $(DESTDIR)$(DRV_LIBDIR)/*.so
	rmdir -p --ignore-fail-on-non-empty $(DESTDIR)$(DRV_LIBDIR)

ifneq ($(wildcard .depend),)
include .depend
endif
