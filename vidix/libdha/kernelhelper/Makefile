KERNEL_INCLUDES = /usr/src/linux/include
INCLUDES = -I$(KERNEL_INCLUDES)
CFLAGS = -O2 -Wall -D__KERNEL__ -DMODULE -include $(KERNEL_INCLUDES)/linux/modversions.h
CONFIG_SHELL=/bin/sh -c
KERNEL_VERSION=$(shell $(CONFIG_SHELL) 'uname -r')
MDIR = /lib/modules/$(KERNEL_VERSION)/misc

all: dhahelper.o test

dhahelper.o: dhahelper.c dhahelper.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(basename $@).c

test: test.c
	$(CC) -g -O -Wall $(INCLUDES) -o $@ $@.c

install: dhahelper.o
	if test ! -d $(MDIR) ; then mkdir -p $(MDIR) ; fi
	install -m 644 dhahelper.o $(MDIR)/dhahelper.o
	depmod -a
	rm -f /dev/dhahelper
	mknod -m 666 /dev/dhahelper c 252 0


nodes:
	echo Obsolete!!!

dep:

clean: 
	rm -f *.o *~

distclean: clean
	rm -f test
