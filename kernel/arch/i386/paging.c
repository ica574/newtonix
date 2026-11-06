#include <stdint.h>
#include <string.h>

#include <kernel/paging.h>
#include <kernel/pmm.h>

#define PAGE_ENTRY_COUNT 1024U

#define PAGE_PRESENT (1U << 0)
#define PAGE_WRITEABLE (1U << 1)

void paging_enable(uint32_t page_directory_address);

static uint32_t page_directory[PAGE_ENTRY_COUNT] __attribute__((aligned(PAGE_SIZE)));

static uint32_t first_page_table[PAGE_ENTRY_COUNT] __attribute__((aligned(PAGE_SIZE)));

void paging_initialise(void)
{
    memset(page_directory, 0, sizeof(page_directory));

    for (uint32_t index = 0; index < PAGE_ENTRY_COUNT; index++)
    {
        uint32_t physical_address = index * PAGE_SIZE;

        first_page_table[index] = physical_address | PAGE_PRESENT | PAGE_WRITEABLE;
    }

    page_directory[0] = (uint32_t)first_page_table | PAGE_PRESENT | PAGE_WRITEABLE;

    paging_enable((uint32_t)page_directory);
}
