/*
    Direct Hardware Access kernel helper
    
    (C) 2002 Alex Beregszaszi <alex@naxine.org>
    (C) 2002 Nick Kurshev <nickols_k@mail.ru>

    Accessing hardware from userspace as USER (no root needed!)

    Tested on 2.2.x (2.2.19) and 2.4.x (2.4.3,2.4.17).
    
    License: GPL
    
    WARNING! THIS MODULE VIOLATES SEVERAL SECURITY LINES! DON'T USE IT
    ON PRODUCTION SYSTEMS, ONLY AT HOME, ON A "SINGLE-USER" SYSTEM.
    NO WARRANTY!

    Tech:
	Communication between userspace and kernelspace goes over character
	device using ioctl.

    Usage:
	mknod -m 666 /dev/dhahelper c 180 0
	
	Also you can change the major number, setting the "dhahelper_major"
	module parameter, the default is 180, specified in dhahelper.h.
	
	Note: do not use other than minor==0, the module forbids it.

    TODO:
	* do memory mapping without fops:mmap
	* implement unmap memory
	* select (request?) a "valid" major number (from Linux project? ;)
	* make security
	* is pci handling needed? (libdha does this with lowlevel port funcs)
	* is mttr handling needed?
	* test on older kernels (2.0.x (?))
*/

#ifndef MODULE
#define MODULE
#endif

#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <linux/config.h>

#ifdef CONFIG_MODVERSION
#define MODVERSION
#include <linux/modversions.h>
#endif

#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/wrapper.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <asm/pgtable.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
#include <linux/malloc.h>
#else
#include <linux/slab.h>
#endif

#include <linux/pci.h>
#include <linux/ioport.h>
#include <linux/init.h>

#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>

#include <linux/mman.h>

#include <linux/fs.h>
#include <linux/unistd.h>

#include "dhahelper.h"

MODULE_AUTHOR("Alex Beregszaszi <alex@naxine.org>");
MODULE_DESCRIPTION("Provides userspace access to hardware (security violation!)");
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

static int dhahelper_major = DEFAULT_MAJOR;
MODULE_PARM(dhahelper_major, "i");
MODULE_PARM_DESC(dhahelper_major, "Major number of dhahelper characterdevice");

/* 0 = silent */
/* 1 = report errors (default) */
/* 2 = debug */
static int dhahelper_verbosity = 1;
MODULE_PARM(dhahelper_verbosity, "i");
MODULE_PARM_DESC(dhahelper_verbosity, "Level of verbosity (0 = silent, 1 = only errors, 2 = debug)");

static dhahelper_memory_t last_mem_request;


static int dhahelper_open(struct inode *inode, struct file *file)
{
    if (dhahelper_verbosity > 1)
	printk(KERN_DEBUG "dhahelper: device opened\n");

    if (MINOR(inode->i_rdev) != 0)
	return -ENXIO;

    MOD_INC_USE_COUNT;

    return 0;
}

static int dhahelper_release(struct inode *inode, struct file *file)
{
    if (dhahelper_verbosity > 1)
	printk(KERN_DEBUG "dhahelper: device released\n");

    if (MINOR(inode->i_rdev) != 0)
	return -ENXIO;

    MOD_DEC_USE_COUNT;

    return 0;
}

static int dhahelper_get_version(int * arg)
{
	int version = API_VERSION;

	if (copy_to_user(arg, &version, sizeof(int)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy to userspace\n");
		return -EFAULT;
	}
	return 0;
}

static int dhahelper_port(dhahelper_port_t * arg)
{
	dhahelper_port_t port;
	if (copy_from_user(&port, arg, sizeof(dhahelper_port_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy from userspace\n");
		return -EFAULT;
	}
	switch(port.operation)
	{
		case PORT_OP_READ:
		{
		    switch(port.size)
		    {
			case 1:
			    port.value = inb(port.addr);
			    break;
			case 2:
			    port.value = inw(port.addr);
			    break;
			case 4:
			    port.value = inl(port.addr);
			    break;
			default:
			    if (dhahelper_verbosity > 0)
				printk(KERN_ERR "dhahelper: invalid port read size (%d)\n",
				    port.size);
			    return -EINVAL;
		    }
		    break;
		}
		case PORT_OP_WRITE:
		{
		    switch(port.size)
		    {
			case 1:
			    outb(port.value, port.addr);
			    break;
			case 2:
			    outw(port.value, port.addr);
			    break;
			case 4:
			    outl(port.value, port.addr);
			    break;
			default:
			    if (dhahelper_verbosity > 0)
				printk(KERN_ERR "dhahelper: invalid port write size (%d)\n",
				    port.size);
			    return -EINVAL;
		    }
		    break;
		}
		default:
		    if (dhahelper_verbosity > 0)
		        printk(KERN_ERR "dhahelper: invalid port operation (%d)\n",
		    	    port.operation);
		    return -EINVAL;
	}
	/* copy back only if read was performed */
	if (port.operation == PORT_OP_READ)
	if (copy_to_user(arg, &port, sizeof(dhahelper_port_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy to userspace\n");
		return -EFAULT;
	}
	return 0;
}

static int dhahelper_memory(dhahelper_memory_t * arg)
{
	dhahelper_memory_t mem;
	if (copy_from_user(&mem, arg, sizeof(dhahelper_memory_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy from userspace\n");
		return -EFAULT;
	}
	switch(mem.operation)
	{
		case MEMORY_OP_MAP:
		{
#if 1
		    memcpy(&last_mem_request, &mem, sizeof(dhahelper_memory_t));
#else
		    mem.ret = do_mmap(file, mem.start, mem.size, PROT_READ|PROT_WRITE,
			MAP_SHARED, mem.offset);
#endif
		    break;
		}
		case MEMORY_OP_UNMAP:
		    break;
		default:
		    if (dhahelper_verbosity > 0)
			printk(KERN_ERR "dhahelper: invalid memory operation (%d)\n",
			    mem.operation);
		    return -EINVAL;
	}
	if (copy_to_user(arg, &mem, sizeof(dhahelper_memory_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy to userspace\n");
		return -EFAULT;
	}
	return 0;
}

/*******************************/
/* Memory management functions */
/* from kernel:/drivers/mdia/video/bttv-driver.c */
/*******************************/

#define MDEBUG(x)	do { } while(0)		/* Debug memory management */

/* [DaveM] I've recoded most of this so that:
 * 1) It's easier to tell what is happening
 * 2) It's more portable, especially for translating things
 *    out of vmalloc mapped areas in the kernel.
 * 3) Less unnecessary translations happen.
 *
 * The code used to assume that the kernel vmalloc mappings
 * existed in the page tables of every process, this is simply
 * not guarenteed.  We now use pgd_offset_k which is the
 * defined way to get at the kernel page tables.
 */

/* Given PGD from the address space's page table, return the kernel
 * virtual mapping of the physical memory mapped at ADR.
 */
static inline unsigned long uvirt_to_kva(pgd_t *pgd, unsigned long adr)
{
        unsigned long ret = 0UL;
	pmd_t *pmd;
	pte_t *ptep, pte;
  
	if (!pgd_none(*pgd)) {
                pmd = pmd_offset(pgd, adr);
                if (!pmd_none(*pmd)) {
                        ptep = pte_offset(pmd, adr);
                        pte = *ptep;
                        if(pte_present(pte)) {
				ret  = (unsigned long) page_address(pte_page(pte));
				ret |= (adr & (PAGE_SIZE - 1));
				
			}
                }
        }
        MDEBUG(printk("uv2kva(%lx-->%lx)", adr, ret));
	return ret;
}

static inline unsigned long uvirt_to_bus(unsigned long adr) 
{
        unsigned long kva, ret;

        kva = uvirt_to_kva(pgd_offset(current->mm, adr), adr);
	ret = virt_to_bus((void *)kva);
        MDEBUG(printk("uv2b(%lx-->%lx)", adr, ret));
        return ret;
}

static inline unsigned long uvirt_to_pa(unsigned long adr) 
{
        unsigned long kva, ret;

        kva = uvirt_to_kva(pgd_offset(current->mm, adr), adr);
	ret = virt_to_phys((void *)kva);
        MDEBUG(printk("uv2b(%lx-->%lx)", adr, ret));
        return ret;
}

static inline unsigned long kvirt_to_bus(unsigned long adr) 
{
        unsigned long va, kva, ret;

        va = VMALLOC_VMADDR(adr);
        kva = uvirt_to_kva(pgd_offset_k(va), va);
	ret = virt_to_bus((void *)kva);
        MDEBUG(printk("kv2b(%lx-->%lx)", adr, ret));
        return ret;
}

/* Here we want the physical address of the memory.
 * This is used when initializing the contents of the
 * area and marking the pages as reserved.
 */
static inline unsigned long kvirt_to_pa(unsigned long adr) 
{
        unsigned long va, kva, ret;

        va = VMALLOC_VMADDR(adr);
        kva = uvirt_to_kva(pgd_offset_k(va), va);
	ret = __pa(kva);
        MDEBUG(printk("kv2pa(%lx-->%lx)", adr, ret));
        return ret;
}

static void * rvmalloc(signed long size)
{
	void * mem;
	unsigned long adr, page;

	mem=vmalloc_32(size);
	if (mem) 
	{
		memset(mem, 0, size); /* Clear the ram out, no junk to the user */
	        adr=(unsigned long) mem;
		while (size > 0) 
                {
	                page = kvirt_to_pa(adr);
			mem_map_reserve(virt_to_page(__va(page)));
			adr+=PAGE_SIZE;
			size-=PAGE_SIZE;
		}
	}
	return mem;
}

static void rvfree(void * mem, signed long size)
{
        unsigned long adr, page;
        
	if (mem) 
	{
	        adr=(unsigned long) mem;
		while (size > 0) 
                {
	                page = kvirt_to_pa(adr);
			mem_map_unreserve(virt_to_page(__va(page)));
			adr+=PAGE_SIZE;
			size-=PAGE_SIZE;
		}
		vfree(mem);
	}
}


static int dhahelper_virt_to_phys(dhahelper_vmi_t *arg)
{
	dhahelper_vmi_t mem;
	unsigned long i,nitems;
	char *addr;
	if (copy_from_user(&mem, arg, sizeof(dhahelper_vmi_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy from userspace\n");
		return -EFAULT;
	}
	nitems = mem.length / PAGE_SIZE;
	if(mem.length % PAGE_SIZE) nitems++;
	addr = mem.virtaddr;
	for(i=0;i<nitems;i++)
	{
	    unsigned long result;
	    result = uvirt_to_pa((unsigned long)addr);
	    if (copy_to_user(&mem.realaddr[i], &result, sizeof(unsigned long)))
	    {
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy to userspace\n");
		return -EFAULT;
	    }
	    addr += PAGE_SIZE;
	}
	return 0;
}

static int dhahelper_virt_to_bus(dhahelper_vmi_t *arg)
{
	dhahelper_vmi_t mem;
	unsigned long i,nitems;
	char *addr;
	if (copy_from_user(&mem, arg, sizeof(dhahelper_vmi_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy from userspace\n");
		return -EFAULT;
	}
	nitems = mem.length / PAGE_SIZE;
	if(mem.length % PAGE_SIZE) nitems++;
	addr = mem.virtaddr;
	for(i=0;i<nitems;i++)
	{
	    unsigned long result;
	    result = uvirt_to_bus((unsigned long)addr);
	    if (copy_to_user(&mem.realaddr[i], &result, sizeof(unsigned long)))
	    {
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy to userspace\n");
		return -EFAULT;
	    }
	    addr += PAGE_SIZE;
	}
	return 0;
}


static int dhahelper_alloc_pa(dhahelper_mem_t *arg)
{
	dhahelper_mem_t mem;
	if (copy_from_user(&mem, arg, sizeof(dhahelper_mem_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy from userspace\n");
		return -EFAULT;
	}
	mem.addr = rvmalloc(mem.length);
	if (copy_to_user(arg, &mem, sizeof(dhahelper_mem_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy to userspace\n");
		return -EFAULT;
	}
	return 0;
}

static int dhahelper_free_pa(dhahelper_mem_t *arg)
{
	dhahelper_mem_t mem;
	if (copy_from_user(&mem, arg, sizeof(dhahelper_mem_t)))
	{
		if (dhahelper_verbosity > 0)
		    printk(KERN_ERR "dhahelper: failed copy from userspace\n");
		return -EFAULT;
	}
	rvfree(mem.addr,mem.length);
	return 0;
}

static int dhahelper_ioctl(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    if (dhahelper_verbosity > 1)
	printk(KERN_DEBUG "dhahelper: ioctl(cmd=%x, arg=%lx)\n",
	    cmd, arg);

    if (MINOR(inode->i_rdev) != 0)
	return -ENXIO;

    switch(cmd)
    {
	case DHAHELPER_GET_VERSION: return dhahelper_get_version((int *)arg);
	case DHAHELPER_PORT:	    return dhahelper_port((dhahelper_port_t *)arg);
	case DHAHELPER_MEMORY:      return dhahelper_memory((dhahelper_memory_t *)arg);
	case DHAHELPER_VIRT_TO_PHYS:return dhahelper_virt_to_phys((dhahelper_vmi_t *)arg);
	case DHAHELPER_VIRT_TO_BUS: return dhahelper_virt_to_bus((dhahelper_vmi_t *)arg);
	case DHAHELPER_ALLOC_PA:return dhahelper_alloc_pa((dhahelper_mem_t *)arg);
	case DHAHELPER_FREE_PA: return dhahelper_free_pa((dhahelper_mem_t *)arg);
	default:
    	    if (dhahelper_verbosity > 0)
		printk(KERN_ERR "dhahelper: invalid ioctl (%x)\n", cmd);
	    return -EINVAL;
    }
    return 0;
}

static int dhahelper_mmap(struct file *file, struct vm_area_struct *vma)
{
    if (last_mem_request.operation != MEMORY_OP_MAP)
    {
	if (dhahelper_verbosity > 0)
	    printk(KERN_ERR "dhahelper: mapping not requested before mmap\n");
	return -EFAULT;
    }
    
    if (dhahelper_verbosity > 1)
	printk(KERN_INFO "dhahelper: mapping %x (size: %x)\n",
	    last_mem_request.start+last_mem_request.offset, last_mem_request.size);
    
    if (remap_page_range(0, last_mem_request.start + last_mem_request.offset,
	last_mem_request.size, vma->vm_page_prot))
    {
	if (dhahelper_verbosity > 0)
	    printk(KERN_ERR "dhahelper: error mapping memory\n");
	return -EFAULT;
    }

    return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
static struct file_operations dhahelper_fops =
{
    /*llseek*/	NULL,
    /*read*/	NULL,
    /*write*/	NULL,
    /*readdir*/	NULL,
    /*poll*/	NULL,
    /*ioctl*/	dhahelper_ioctl,
    /*mmap*/	dhahelper_mmap,
    /*open*/	dhahelper_open,
    /*flush*/	NULL,
    /*release*/	dhahelper_release,
    /* zero out the last 5 entries too ? */
};
#else
static struct file_operations dhahelper_fops =
{
    owner:	THIS_MODULE,
    ioctl:	dhahelper_ioctl,
    mmap:	dhahelper_mmap,
    open:	dhahelper_open,
    release:	dhahelper_release
};
#endif

#if KERNEL_VERSION < KERNEL_VERSION(2,4,0)
int init_module(void)
#else 
static int __init init_dhahelper(void)
#endif
{
    printk(KERN_INFO "Direct Hardware Access kernel helper (C) Alex Beregszaszi\n");

    if(register_chrdev(dhahelper_major, "dhahelper", &dhahelper_fops))
    {
    	if (dhahelper_verbosity > 0)
	    printk(KERN_ERR "dhahelper: unable to register character device (major: %d)\n",
		dhahelper_major);
	return -EIO;
    }
    
    return 0;
}

#if KERNEL_VERSION < KERNEL_VERSION(2,4,0)
void cleanup_module(void)
#else
static void __exit exit_dhahelper(void)
#endif
{
    unregister_chrdev(dhahelper_major, "dhahelper");
}

EXPORT_NO_SYMBOLS;

#if KERNEL_VERSION >= KERNEL_VERSION(2,4,0)
module_init(init_dhahelper);
module_exit(exit_dhahelper);
#endif
