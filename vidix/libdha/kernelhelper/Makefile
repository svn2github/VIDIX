KERNEL_DIR = /usr/src/linux
KERNEL_INCLUDES = $(KERNEL_DIR)/include
INCLUDES = -I$(KERNEL_INCLUDES)
CFLAGS = -O2 -Wall -D__KERNEL__ -DMODULE -fomit-frame-pointer -fno-common -fno-strict-aliasing -DKBUILD_BASENAME=dhahelper -DKBUILD_MODNAME=dhahelper
CONFIG_SHELL=/bin/sh -c
KERNEL_VERSION=$(shell $(CONFIG_SHELL) 'uname -r')
MDIR = /lib/modules/$(KERNEL_VERSION)/misc
MODPOST = $(KERNEL_DIR)/scripts/modpost

all: test

dhahelper.o: dhahelper.c dhahelper.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(basename $@).c

ifeq ($(wildcard $(MODPOST)),$(MODPOST))
MODULE = dhahelper.ko

dhahelper.ko: dhahelper.mod.o dhahelper.o
	ld -r -o $@ $^

dhahelper.mod.c: dhahelper.o
	$(MODPOST) $(KERNEL_DIR)/vmlinux dhahelper.o
else
MODULE = dhahelper.o
endif

all: $(MODULE)

test: test.c
	$(CC) -g -O -Wall $(INCLUDES) -o $@ $@.c

install: $(MODULE)
	if test ! -d $(MDIR) ; then mkdir -p $(MDIR) ; fi
	install -m 644 $(MODULE) $(MDIR)
	depmod -a
	if [ ! -c /dev/.devfsd ]; then \
	    rm -f /dev/dhahelper; \
	    mknod -m 666 /dev/dhahelper c 252 0; \
	fi


nodes:
	echo Obsolete!!!

dep:

clean: 
	rm -f *.o *~ *.mod.* *.ko

distclean: clean
	rm -f test
