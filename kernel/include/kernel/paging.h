#include <stdint.h>

#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H

#define PAGING_FLAG_WRITABLE (1U << 1)

void paging_initialise(void);

void paging_map_page(uint32_t virtual_address, uint32_t physical_address, uint32_t flags);
uint32_t paging_unmap_page(uint32_t virtual_address);

#endif
