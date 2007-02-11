# makefile

include ../../config.mak

BINDIR = $(prefix)/lib/vidix
OLDBINDIR = $(prefix)/lib/mplayerxp/vidix
ifeq ($(TARGET_OS),CYGWIN)
L_FLAGS=
LINKER=$(CC)
else
OPTFLAGS += -fPIC
L_FLAGS=-soname $@
LINKER=$(LD)
endif

COMMON_CFLAGS = -I. -I.. -Wall -W

RADEON_VID=radeon_vid.so
RADEON_SRCS=radeon_vid.c
RADEON_OBJS=radeon_vid.o
RADEON_LIBS=-L../../libdha -ldha -lm
RADEON_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

RAGE128_VID=rage128_vid.so
RAGE128_SRCS=radeon_vid.c
RAGE128_OBJS=rage128_vid.o
RAGE128_LIBS=-L../../libdha -ldha
RAGE128_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS) -DRAGE128

PM3_VID=pm3_vid.so
PM3_SRCS=pm3_vid.c
PM3_OBJS=pm3_vid.o
PM3_LIBS=-L../../libdha -ldha
PM3_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

PM2_VID=pm2_vid.so
PM2_SRCS=pm2_vid.c
PM2_OBJS=pm2_vid.o
PM2_LIBS=-L../../libdha -ldha
PM2_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

MACH64_VID=mach64_vid.so
MACH64_SRCS=mach64_vid.c
MACH64_OBJS=mach64_vid.o
MACH64_LIBS=-L../../libdha -ldha
MACH64_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS) -DRAGE128

NVIDIA_VID=nvidia_vid.so
NVIDIA_SRCS=nvidia_vid.c
NVIDIA_OBJS=nvidia_vid.o
NVIDIA_LIBS=-L../../libdha -ldha -lm
NVIDIA_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

GENFB_VID=genfb_vid.so
GENFB_SRCS=genfb_vid.c
GENFB_OBJS=genfb_vid.o
GENFB_LIBS=-L../../libdha -ldha -lm
GENFB_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

MGA_VID=mga_vid.so
MGA_SRCS=mga_vid.c
MGA_OBJS=mga_vid.o
MGA_LIBS=-L../../libdha -ldha -lm
MGA_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

MGA_CRTC2_VID=mga_crtc2_vid.so
MGA_CRTC2_SRCS=mga_vid.c
MGA_CRTC2_OBJS=mga_crtc2_vid.o
MGA_CRTC2_LIBS=-L../../libdha -ldha -lm
MGA_CRTC2_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS) -DCRTC2

MGA_TV_VID=mga_tv_vid.so
MGA_TV_SRCS=mga_vid.c
MGA_TV_OBJS=mga_tv_vid.o
MGA_TV_LIBS=-L../../libdha -ldha -lm
MGA_TV_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS) -DMGA_TV

SIS_VID=sis_vid.so
SIS_SRCS=sis_vid.c sis_bridge.c
SIS_OBJS=sis_vid.o sis_bridge.o
SIS_LIBS=-L../../libdha -ldha
SIS_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

UNICHROME_VID=unichrome_vid.so
UNICHROME_SRCS=unichrome_vid.c
UNICHROME_OBJS=unichrome_vid.o
UNICHROME_LIBS=-L../../libdha -ldha
UNICHROME_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

CYBERBLADE_VID=cyberblade_vid.so
CYBERBLADE_SRCS=cyberblade_vid.c
CYBERBLADE_OBJS=cyberblade_vid.o
CYBERBLADE_LIBS=-L../../libdha -ldha -lm
CYBERBLADE_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

SAVAGE_VID=savage_vid.so
SAVAGE_SRCS=savage_vid.c
SAVAGE_OBJS=savage_vid.o
SAVAGE_LIBS=-L../../libdha -ldha -lm
SAVAGE_CFLAGS=$(OPTFLAGS) $(COMMON_CFLAGS)

all:    $(RADEON_VID) $(RAGE128_VID) $(MACH64_VID) $(NVIDIA_VID) $(GENFB_VID) $(MGA_VID) $(MGA_CRTC2_VID) $(MGA_TV_VID) $(PM3_VID) $(PM2_VID) $(SIS_VID) $(UNICHROME_VID) $(CYBERBLADE_VID) $(SAVAGE_VID)


.SUFFIXES: .c .o

# .PHONY: all clean

$(PM3_OBJS):    $(PM3_SRCS)
	$(CC) -c $(PM3_CFLAGS) -o $@ $<

$(PM3_VID):     $(PM3_OBJS)
	$(LINKER) $(PM3_LIBS) -shared $(L_FLAGS) -o $(PM3_VID) $(PM3_OBJS)

$(PM2_OBJS):    $(PM2_SRCS)
	$(CC) -c $(PM2_CFLAGS) -o $@ $<

$(PM2_VID):     $(PM2_OBJS)
	$(LINKER) $(PM2_LIBS) -shared $(L_FLAGS) -o $(PM2_VID) $(PM2_OBJS)

$(RADEON_OBJS):    $(RADEON_SRCS)
	$(CC) -c $(RADEON_CFLAGS) -o $@ $<

$(RADEON_VID):     $(RADEON_OBJS)
	$(LINKER) $(RADEON_LIBS) -shared $(L_FLAGS) -o $(RADEON_VID) $(RADEON_OBJS)

$(RAGE128_OBJS):    $(RAGE128_SRCS)
	$(CC) -c $(RAGE128_CFLAGS) -o $@ $<

$(RAGE128_VID):     $(RAGE128_OBJS)
	$(LINKER) $(RAGE128_LIBS) -shared $(L_FLAGS) -o $(RAGE128_VID) $(RAGE128_OBJS)

$(MACH64_OBJS):    $(MACH64_SRCS)
	$(CC) -c $(MACH64_CFLAGS) -o $@ $<

$(MACH64_VID):     $(MACH64_OBJS)
	$(LINKER) $(MACH64_LIBS) -shared $(L_FLAGS) -o $(MACH64_VID) $(MACH64_OBJS)

$(NVIDIA_OBJS):    $(NVIDIA_SRCS)
	$(CC) -c $(NVIDIA_CFLAGS) -o $@ $<

$(NVIDIA_VID):     $(NVIDIA_OBJS)
	$(LINKER) $(NVIDIA_LIBS) -shared $(L_FLAGS) -o $(NVIDIA_VID) $(NVIDIA_OBJS)

$(GENFB_OBJS):    $(GENFB_SRCS)
	$(CC) -c $(GENFB_CFLAGS) -o $@ $<

$(GENFB_VID):     $(GENFB_OBJS)
	$(LINKER) $(GENFB_LIBS) -shared $(L_FLAGS) -o $(GENFB_VID) $(GENFB_OBJS)

$(MGA_OBJS):    $(MGA_SRCS)
	$(CC) -c $(MGA_CFLAGS) -o $@ $<

$(MGA_VID):     $(MGA_OBJS)
	$(LINKER) $(MGA_LIBS) -shared $(L_FLAGS) -o $(MGA_VID) $(MGA_OBJS)

$(MGA_CRTC2_OBJS):    $(MGA_CRTC2_SRCS)
	$(CC) -c $(MGA_CRTC2_CFLAGS) -o $@ $<

$(MGA_CRTC2_VID):     $(MGA_CRTC2_OBJS)
	$(LINKER) $(MGA_CRTC2_LIBS) -shared $(L_FLAGS) -o $(MGA_CRTC2_VID) $(MGA_CRTC2_OBJS)

$(MGA_TV_OBJS):    $(MGA_TV_SRCS)
	$(CC) -c $(MGA_TV_CFLAGS) -o $@ $<

$(MGA_TV_VID):     $(MGA_TV_OBJS)
	$(LINKER) $(MGA_TV_LIBS) -shared $(L_FLAGS) -o $(MGA_TV_VID) $(MGA_TV_OBJS)

$(SIS_OBJS):    $(SIS_SRCS)
	$(CC) -c $(SIS_CFLAGS) -o $@ $(basename $@).c

$(SIS_VID):     $(SIS_OBJS)
	$(LINKER) $(SIS_LIBS) -shared $(L_FLAGS) -o $(SIS_VID) $(SIS_OBJS) 

$(UNICHROME_OBJS):    $(UNICHROME_SRCS)
	$(CC) -c $(UNICHROME_CFLAGS) -o $@ $(basename $@).c

$(UNICHROME_VID):     $(UNICHROME_OBJS)
	$(LINKER) $(UNICHROME_LIBS) -shared $(L_FLAGS) -o $(UNICHROME_VID) $(UNICHROME_OBJS) 

$(CYBERBLADE_OBJS):    $(CYBERBLADE_SRCS)
	$(CC) -c $(CYBERBLADE_CFLAGS) -o $@ $<

$(CYBERBLADE_VID):     $(CYBERBLADE_OBJS)
	$(LINKER) $(CYBERBLADE_LIBS) -shared $(L_FLAGS) -o $(CYBERBLADE_VID) $(CYBERBLADE_OBJS) 

$(SAVAGE_OBJS):    $(SAVAGE_SRCS)
	$(CC) -c $(SAVAGE_CFLAGS) -o $@ $<

$(SAVAGE_VID):     $(SAVAGE_OBJS)
	$(LINKER) $(SAVAGE_LIBS) -shared $(L_FLAGS) -o $(SAVAGE_VID) $(SAVAGE_OBJS) 

clean:
	rm -f *.o *.so *~

distclean:
	rm -f Makefile.bak *.o *.so test *~ .depend

dep:    depend

depend:
# do nothing here

install:
	mkdir -p $(BINDIR)
	install -m 755 -p *.so $(BINDIR)
	if test -d $(OLDBINDIR) ; then rm -f $(OLDBINDIR)/*.so ; rmdir -p --ignore-fail-on-non-empty $(OLDBINDIR) ; fi
uninstall:
	rm -f $(BINDIR)/*.so
	rmdir -p --ignore-fail-on-non-empty $(BINDIR)

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
