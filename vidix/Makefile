SUBDIRS = libdha vidix

DO_MAKE = @ for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit; done

all:
	$(DO_MAKE)

install:
	$(DO_MAKE)

clean:
	$(DO_MAKE)

distclean:
	$(DO_MAKE)
	rm -f config.h config.mak configure.log

uninstall:
	$(DO_MAKE)

dep:
	$(DO_MAKE)

