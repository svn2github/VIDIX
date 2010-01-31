include ../../config.mak

DRV_LIBDIR = $(LIBDIR)/vidix
OLDLIBDIR = $(prefix)/lib/mplayerxp/vidix

VIDIX_LIBS = -L../../libdha -ldha
VIDIX_LIBS_M = $(VIDIX_LIBS) -lm
VIDIX_LIBS_X = $(VIDIX_LIBS_M) $(X_LIB)
VIDIX_CFLAGS = -W -Wall -fPIC -I. -I.. -I../.. $(OPTFLAGS)

MACH64_CFLAGS = $(VIDIX_CFLAGS) -DRAGE128
RAGE128_CFLAGS = $(VIDIX_CFLAGS) -DRAGE128
MGA_CRTC2_CFLAGS = $(VIDIX_CFLAGS) -DCRTC2
MGA_TV_CFLAGS = $(VIDIX_CFLAGS) -DMGA_TV

DRV=    cyberblade_vid.so \
	genfb_vid.so \
	mach64_vid.so \
	mga_crtc2_vid.so \
	mga_tv_vid.so \
	mga_vid.so \
	nvidia_vid.so \
	pm2_vid.so \
	pm3_vid.so \
	radeon_vid.so \
	rage128_vid.so \
	radeonhd_vid.so \
	s3_vid.so \
	savage_vid.so \
	sis_vid.so \
	unichrome_vid.so
ifeq ($(TARGET_OS),Linux)
ifeq ($(HAVE_IVTV),yes)
DRV += ivtv_vid.so
endif
endif

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

ifeq ($(VIDIX_BUILD_STATIC),yes)
all: $(OBJS)
endif
ifeq ($(VIDIX_BUILD_SHARED),yes)
all: $(DRV)
endif

.SUFFIXES: .c .o

# .PHONY: all clean

# Object files
%.o: %.c
	$(CC) -c $(VIDIX_CFLAGS) -o $@ $<

# Dynamic librairies
%.so: %.o
	$(CC) $(LDFLAGS) -shared $^ $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

mga_crtc2_vid.o: mga_vid.c
	$(CC) -c $(MGA_CRTC2_CFLAGS) -o $@ $<
mga_tv_vid.o: mga_vid.c
	$(CC) -c $(MGA_TV_CFLAGS) -o $@ $<
rage128_vid.o: radeon_vid.c
	$(CC) -c $(RAGE128_CFLAGS) -o $@ $<


clean:
	rm -f *.o *.so *~

distclean:
	rm -f *.o *.so *~ .depend

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
