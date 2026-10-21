#ifndef KERNEL_PMM_H
#define KERNEL_PMM_H

#include <stdint.h>

#define PAGE_SIZE 4096U

void pmm_initialize(void);
void pmm_add_usable_region(uint64_t address, uint64_t length);
void pmm_reserve_region(uint64_t address, uint64_t length);
uint32_t pmm_allocate_page(void);
void pmm_free_page(uint32_t address);
uint32_t pmm_count_free_pages(void);

#endif
