/*
    Generic stuff to compile VIDIX only on any system (SCRATCH)
*/
#warn This stuff is not ported on yur system

static int pci_config_type( void )
{
    printf("pci_config_type: generic function call\n");
    return 0xFFFF;
}

static int pci_get_vendor(
          unsigned char bus,
          unsigned char dev,
          int func)
{
    printf("pci_get_vendor: generic function call\n");
    return 0;
}

static long pci_config_read_long(
          unsigned char bus,
          unsigned char dev,
          int func, 
          unsigned cmd)
{
    printf("pci_config_read_long: generic function call\n");
    return 0;
}

static long pci_config_read_word(
          unsigned char bus,
          unsigned char dev,
          int func, 
          unsigned cmd)
{
    printf("pci_config_read_word: generic function call\n");
    return 0;
}

static long pci_config_read_byte(
          unsigned char bus,
          unsigned char dev,
          int func, 
          unsigned cmd)
{
    printf("pci_config_read_byte: generic function call\n");
    return 0;
}
