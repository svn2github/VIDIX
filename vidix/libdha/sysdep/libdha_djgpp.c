#include <dpmi.h>
#include <sys/nearptr.h>

#define MAP_FAILED ((void *) -1)

void *map_phys_mem(unsigned long base, unsigned long size)
{
   __dpmi_meminfo meminfo;
   unsigned long linear;

   printf ("vidixlib: mapping %08x:%08x\n", base, size);
   meminfo.address = base;
   meminfo.size = size;
   if (__dpmi_physical_address_mapping(&meminfo))
     return MAP_FAILED;

   linear = meminfo.address + __djgpp_conventional_base;
//   __dpmi_lock_linear_region(&meminfo);
   printf ("vidixlib: into %08x\n", linear);
   return (void *)(linear);
}

void unmap_phys_mem(void *ptr, unsigned long size)
{
   __dpmi_meminfo meminfo;
   unsigned long linear = ptr;
   
   printf ("vidixlib: unmapping %08x\n", linear);
   if (linear)
   {
     meminfo.address = linear;
     __dpmi_free_physical_address_mapping(&meminfo);
     ptr = NULL;
   }
}
