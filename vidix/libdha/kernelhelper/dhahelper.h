/*
    Direct Hardware Access kernel helper
    
    (C) 2002 Alex Beregszaszi <alex@naxine.org>
*/

#ifndef DHAHELPER_H
#define DHAHELPER_H

#include <linux/ioctl.h>

/* feel free to change */
#define DEFAULT_MAJOR	180

#define API_VERSION	0x10 /* 1.0*/

typedef struct dhahelper_port_s
{
#define PORT_OP_READ	1
#define PORT_OP_WRITE	2
    int		operation;
    int		size;
    int      	addr; // FIXME - switch to void* (64bit)
    int		value;
} dhahelper_port_t;

typedef struct dhahelper_mtrr_s
{
#define MTRR_OP_ADD	1
#define MTRR_OP_DEL	2
    int		operation;
    int		start;
    int		size;
    int		type;
} dhahelper_mtrr_t;

typedef struct dhahelper_pci_s
{
#define PCI_OP_READ	1
#define PCI_OP_WRITE	1
    int		operation;
    int		bus;
    int		dev;
    int		func;
    int		cmd;
    int		size;
    int		ret;
} dhahelper_pci_t;

typedef struct dhahelper_vmi_s
{
    void *	virtaddr;
    unsigned long length;
    unsigned long *realaddr;
}dhahelper_vmi_t;

typedef struct dhahelper_mem_s
{
    void *	addr;
    unsigned long length;
}dhahelper_mem_t;

#define DHAHELPER_GET_VERSION	_IOW('D', 0, int)
#define DHAHELPER_PORT		_IOWR('D', 1, dhahelper_port_t)
#define DHAHELPER_MTRR		_IOWR('D', 2, dhahelper_mtrr_t)
#define DHAHELPER_PCI		_IOWR('D', 3, dhahelper_pci_t)
#define DHAHELPER_VIRT_TO_PHYS	_IOWR('D', 4, dhahelper_vmi_t)
#define DHAHELPER_VIRT_TO_BUS	_IOWR('D', 5, dhahelper_vmi_t)
#define DHAHELPER_ALLOC_PA	_IOWR('D', 6, dhahelper_mem_t)
#define DHAHELPER_FREE_PA	_IOWR('D', 7, dhahelper_mem_t)
#define DHAHELPER_LOCK_MEM	_IOWR('D', 8, dhahelper_mem_t)
#define DHAHELPER_UNLOCK_MEM	_IOWR('D', 9, dhahelper_mem_t)

#endif /* DHAHELPER_H */
