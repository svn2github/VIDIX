/* Memory manager interface */
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h> /* mlock */
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "config.h"
#include "libdha.h"
#ifdef CONFIG_LINUXHELPER
#include "kernelhelper/dhahelper.h"
#endif
static int libdha_fd=-1;

#ifndef HAVE_MLOCK
/* stubs */
int mlock(const void *addr,size_t len) { return ENOSYS; }
int munlock(const void *addr,size_t len) { return ENOSYS; }
#endif

#define ALLOWED_VER 0x10
int bm_open( void )
{
  int retv=ENXIO;
#ifdef CONFIG_LINUXHELPER
  libdha_fd = open("/dev/dhahelper",O_RDWR);
  retv = libdha_fd > 0 ? 0 : ENXIO;
  if(!retv)
  {
    int ver;
    ioctl(libdha_fd,DHAHELPER_GET_VERSION,&ver);
    if(ver < ALLOWED_VER)
    {
	printf("libdha: You have wrong version (%i) of /dev/dhahelper\n"
	       "libdha: Please upgrade your driver up to ver=%i\n",ver,ALLOWED_VER);
	retv = EINVAL;
	close(libdha_fd);
    }
  }
  else printf("libdha: Can't open /dev/dhahelper\n");
#endif
  return retv;
}

void bm_close( void )
{
  close(libdha_fd);
}

int bm_virt_to_phys( void * virt_addr, unsigned long length, unsigned long * parray )
{
#ifdef CONFIG_LINUXHELPER
    dhahelper_vmi_t vmi;
    vmi.virtaddr = virt_addr;
    vmi.length = length;
    vmi.realaddr = parray;
    if(libdha_fd > 0) return ioctl(libdha_fd,DHAHELPER_VIRT_TO_PHYS,&vmi);
#endif
    return ENXIO;
}

int bm_virt_to_bus( void * virt_addr, unsigned long length, unsigned long * barray )
{
#ifdef CONFIG_LINUXHELPER
    dhahelper_vmi_t vmi;
    vmi.virtaddr = virt_addr;
    vmi.length = length;
    vmi.realaddr = barray;
    if(libdha_fd > 0) return ioctl(libdha_fd,DHAHELPER_VIRT_TO_BUS,&vmi);
#endif
    return ENXIO;
}

void *	bm_alloc_pci_shmem(pciinfo_t *pi __attribute__ ((unused)), unsigned mem_bitness __attribute__ ((unused)), unsigned long length __attribute__ ((unused)),int op __attribute__ ((unused)) )
{
    printf("libdha: Pure virtual function call - bm_alloc_pci_shmem()\n");
#if 0
    dhahelper_mem_t vmi;
    vmi.length = length;
    if(libdha_fd > 0) 
    {
	if(ioctl(libdha_fd,DHAHELPER_ALLOC_PA,&vmi) == 0)
		return vmi.addr;
    }
#endif
    return NULL;
}

void	bm_free_pci_shmem(void * pci_shmem __attribute__ ((unused)))
{
    printf("libdha: Pure virtual function call - bm_free_pci_shmem()\n");
#if 0
    dhahelper_mem_t vmi;
    vmi.addr = virt_addr;
    vmi.length = length;
    if(libdha_fd > 0) 
    {
	ioctl(libdha_fd,DHAHELPER_FREE_PA,&vmi);
    }
#endif
}

int	bm_lock_mem( const void *addr, unsigned long length )
{
#ifdef CONFIG_LINUXHELPER
    dhahelper_mem_t vmi;
    vmi.addr = (void *) addr;
    vmi.length = length;
    if(libdha_fd > 0) 
    {
	return ioctl(libdha_fd,DHAHELPER_LOCK_MEM,&vmi);
    }
#endif
    return mlock(addr,length);
}

int	bm_unlock_mem( const void * addr, unsigned long length )
{
#ifdef CONFIG_LINUXHELPER
    dhahelper_mem_t vmi;
    vmi.addr = (void *) addr;
    vmi.length = length;
    if(libdha_fd > 0) 
    {
	return ioctl(libdha_fd,DHAHELPER_UNLOCK_MEM,&vmi);
    }
#endif
    return munlock(addr,length);
}
