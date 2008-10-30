include ../../config.mak

DRV_LIBDIR = $(LIBDIR)/vidix
OLDLIBDIR = $(prefix)/lib/mplayerxp/vidix

VIDIX_LIBS = -L../../libdha -ldha
VIDIX_LIBS_M = $(VIDIX_LIBS) -lm
VIDIX_LIBS_X = $(VIDIX_LIBS_M) $(X_LIB)
VIDIX_CFLAGS = -fPIC -I. -I.. -I../.. $(OPTFLAGS)

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
	s3_vid.so \
	savage_vid.so \
	sis_vid.so \
	unichrome_vid.so \

ifeq ($(TARGET_OS),Linux)
ifeq ($(HAVE_IVTV),yes)
DRV += ivtv_vid.so
endif
endif

all: $(DRV)

.SUFFIXES: .c .o

# .PHONY: all clean

# Object files
%.o: %.c
	$(CC) -c $(VIDIX_CFLAGS) -o $@ $<

mach64_vid.o: mach64_vid.c
	$(CC) -c $(MACH64_CFLAGS) -o $@ $<

mga_crtc2_vid.o: mga_vid.c
	$(CC) -c $(MGA_CRTC2_CFLAGS) -o $@ $<

mga_tv_vid.o: mga_vid.c
	$(CC) -c $(MGA_TV_CFLAGS) -o $@ $<

rage128_vid.o: radeon_vid.c
	$(CC) -c $(RAGE128_CFLAGS) -o $@ $<

# Dynamic librairies
%.so: %.o
	$(CC) -shared $^ $(VIDIX_LIBS) -Wl,-soname,$@ -o $@

cyberblade_vid.so: cyberblade_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

genfb_vid.so: genfb_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

ivtvfb_vid.so: ivtv_vid.o

mach64_vid.so: mach64_vid.o

mga_crtc2_vid.so: mga_crtc2_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

mga_tv_vid.so: mga_tv_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

mga_vid.so: mga_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

nvidia_vid.so: nvidia_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

pm2_vid.so: pm2_vid.o

pm3_vid.so: pm3_vid.o

radeon_vid.so: radeon_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

rage128_vid.so: rage128_vid.o

s3_vid.so: s3_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

savage_vid.so: savage_vid.o
	$(CC) -shared $< $(VIDIX_LIBS_M) -Wl,-soname,$@ -o $@

sis_vid.so: sis_vid.o sis_bridge.o

unichrome_vid.so: unichrome_vid.o
	$(CC) -shared $< $(VIDIX_LIBS) -Wl,-soname,$@ -o $@

clean:
	rm -f *.o *.so *~

distclean:
	rm -f *.o *.so *~ .depend

dep depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

install:
	mkdir -p $(DRV_LIBDIR)
	install -m 755 -p *.so $(DRV_LIBDIR)
	if test -d $(OLDLIBDIR) ; then rm -f $(OLDLIBDIR)/*.so ; rmdir -p --ignore-fail-on-non-empty $(OLDLIBDIR) ; fi

uninstall:
	rm -f $(DRV_LIBDIR)/*.so
	rmdir -p --ignore-fail-on-non-empty $(DRV_LIBDIR)

ifneq ($(wildcard .depend),)
include .depend
endif
