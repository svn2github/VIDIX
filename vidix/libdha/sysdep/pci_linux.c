/*
   This file is based on:
   $XFree86: xc/programs/Xserver/hw/xfree86/etc/scanpci.c,v 3.34.2.17 1998/11/10 11:55:40 dawes Exp $
   Modified for readability by Nick Kurshev
*/
#include <errno.h>
#ifdef __i386__
#include <sys/perm.h>
#else
#ifdef __powerpc__
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,16)
#include <sys/io.h>
#endif
#else
#ifndef __sparc__
#include <sys/io.h>
#endif
#endif
#endif

#include "config.h"

#ifdef CONFIG_DHAHELPER
#include <fcntl.h>
int dhahelper_initialized = 0;
int dhahelper_fd = 0;
#endif

#if defined(__sparc__) || defined(__powerpc__)
#define iopl(x) (0)
#endif

static __inline__ int enable_os_io(void)
{
#ifdef CONFIG_DHAHELPER
    dhahelper_fd = open("/dev/dhahelper", O_RDWR);
    if (dhahelper_fd > 0)
    {
	dhahelper_initialized = 1;
	return(0);
    }
    dhahelper_initialized = -1;
#endif

    if (iopl(3) != 0)
	return(errno);
    return(0);
}

static __inline__ int disable_os_io(void)
{
#ifdef CONFIG_DHAHELPER
    if (dhahelper_initialized == 1)
	close(dhahelper_fd);
    else
#endif
    if (iopl(0) != 0)
	return(errno);
    return(0);
}

#if (defined(__powerpc__) || defined(__sparc__) || defined(__sparc64__) \
    || defined(__x86_64__)) && defined(__linux__) && !defined(CONFIG_SVGAHELPER)
#define CONFIG_PCI_LINUX_PROC
#endif

#if defined(CONFIG_PCI_LINUX_PROC)

#include <linux/pci.h>
#include "../../bswap.h"

int pci_config_type(void)
{
    return 1;
}

int pci_get_vendor(
          unsigned char bus,
          unsigned char dev,
          int func)
{
    int retval;
    char path[100];
    int fd;
    short vendor, device;
    sprintf(path,"/proc/bus/pci/%02d/%02x.0", bus, dev);
    fd = open(path,O_RDONLY|O_SYNC);
    if (fd == -1) {
	    retval=0xFFFF;
    }
    else if (pread(fd, &vendor, 2, PCI_VENDOR_ID) == 2 &&
             pread(fd, &device, 2, PCI_DEVICE_ID) == 2) {
	    vendor = le2me_16(vendor);
	    device = le2me_16(device);
	    retval = vendor + (device<<16);
    } else {
	    retval = 0xFFFF;
    }   
    if (fd > 0) {
	    close(fd);
    }
    return retval;
}

long pci_config_read_long(
          unsigned char bus,
          unsigned char dev,
          int func, 
          unsigned cmd)
{
    long retval;
    char path[100];
    int fd;
    sprintf(path,"/proc/bus/pci/%02d/%02x.0", bus, dev);
    fd = open(path,O_RDONLY|O_SYNC);
    if (fd == -1) {
	    retval=0;
    }
    else if (pread(fd, &retval, 4, cmd) == 4) {
	    retval = le2me_32(retval);
    } else {
	    retval = 0;
    }   
    if (fd > 0) {
	    close(fd);
    }
    return retval;
}
#endif
