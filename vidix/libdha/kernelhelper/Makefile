KERNEL_VERSION = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(KERNEL_VERSION)/build
MDIR = /lib/modules/$(KERNEL_VERSION)/misc

ifeq ($(findstring 2.6.,$(KERNEL_VERSION)),2.6.)
MODULE = dhahelper.ko
obj-m = dhahelper.o

all:
	@echo Please ignore warnings about overriding SUBDIRS
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
else
KERNEL_INCLUDES = $(KERNEL_DIR)/include
INCLUDES = -I$(KERNEL_INCLUDES)
CFLAGS = -O2 -Wall -D__KERNEL__ -DMODULE -fomit-frame-pointer -fno-common -fno-strict-aliasing
MODULE = dhahelper.o

all: $(MODULE) test

dhahelper.o: dhahelper.c dhahelper.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(basename $@).c
endif

test: test.c
	$(CC) -g -O -Wall $(INCLUDES) -o $@ $@.c

install: $(MODULE)
	test -d $(MDIR) || mkdir -p $(MDIR)
	install -m 644 $(MODULE) $(MDIR)
	depmod -a
	if [ ! -c /dev/.devfsd ]; then		 \
	    rm -f /dev/dhahelper;		 \
	    mknod -m 600 /dev/dhahelper c 252 0; \
	fi


nodes:
	@echo make nodes is obsolete.

dep:

clean: 
	rm -f *.o *~ *.mod.* *.ko

distclean: clean
	rm -f test
