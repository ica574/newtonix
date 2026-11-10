#include <stdint.h>
#include <string.h>

#include <kernel/paging.h>
#include <kernel/panic.h>
#include <kernel/pmm.h>

#define PAGE_ENTRY_COUNT 1024U

#define PAGE_PRESENT (1U << 0)
#define PAGE_WRITEABLE (1U << 1)

#define PAGE_TABLE_POOL_SIZE 16U

void paging_enable(uint32_t page_directory_address);

static uint32_t page_directory[PAGE_ENTRY_COUNT] __attribute__((aligned(PAGE_SIZE)));

static uint32_t first_page_table[PAGE_ENTRY_COUNT] __attribute__((aligned(PAGE_SIZE)));

static uint32_t page_table_pool[PAGE_TABLE_POOL_SIZE][PAGE_ENTRY_COUNT]
    __attribute__((aligned(PAGE_SIZE)));

static uint32_t *page_tables[PAGE_ENTRY_COUNT];

static uint32_t next_free_page_table;

void paging_initialise(void)
{
    memset(page_tables, 0, sizeof(page_tables));
    memset(page_table_pool, 0, sizeof(page_table_pool));
    memset(page_directory, 0, sizeof(page_directory));

    next_free_page_table = 0;

    page_tables[0] = first_page_table;

    for (uint32_t index = 0; index < PAGE_ENTRY_COUNT; index++)
    {
        uint32_t physical_address = index * PAGE_SIZE;

        first_page_table[index] = physical_address | PAGE_PRESENT | PAGE_WRITEABLE;
    }

    page_directory[0] = (uint32_t)first_page_table | PAGE_PRESENT | PAGE_WRITEABLE;

    paging_enable((uint32_t)page_directory);
}

void paging_map_page(uint32_t virtual_address, uint32_t physical_address, uint32_t flags)
{
    if ((virtual_address % PAGE_SIZE) != 0)
    {
        kernel_panic("Virtual address is not page-aligned!");
    }

    if ((physical_address % PAGE_SIZE) != 0)
    {
        kernel_panic("Physical address is not page-aligned!");
    }

    uint32_t directory_index = virtual_address >> 22;
    uint32_t table_index = (virtual_address >> 12) & 0x3FFU;

    uint32_t *page_table = page_tables[directory_index];

    if (page_table == NULL)
    {
        if (next_free_page_table >= PAGE_TABLE_POOL_SIZE)
        {
            kernel_panic("Pool exhausted!");
        }

        page_table = page_table_pool[next_free_page_table];
        next_free_page_table++;

        memset(page_table, 0, PAGE_SIZE);

        page_tables[directory_index] = page_table;

        page_directory[directory_index] = (uint32_t)page_table | PAGE_PRESENT | PAGE_WRITEABLE;
    }

    if (page_table[table_index] & PAGE_PRESENT)
    {
        kernel_panic("Virtual page is already mapped");
    }

    page_table[table_index] = physical_address | (flags & 0xFFFU) | PAGE_PRESENT;

    __asm__ volatile("invlpg (%0)" : : "r"(virtual_address) : "memory");
}

uint32_t paging_unmap_page(uint32_t virtual_address)
{
    if ((virtual_address % PAGE_SIZE) != 0)
    {
        kernel_panic("Virtual address is not page-aligned!");
    }

    uint32_t directory_index = virtual_address >> 22;
    uint32_t table_index = (virtual_address >> 12) & 0x3FFU;

    uint32_t *page_table = page_tables[directory_index];

    if (page_table == NULL)
    {
        kernel_panic("Virtual page has no page table");
    }

    uint32_t entry = page_table[table_index];

    if ((entry & PAGE_PRESENT) == 0)
    {
        kernel_panic("Virtual page is not mapped");
    }

    uint32_t physical_address = entry & 0xFFFFF000U;

    page_table[table_index] = 0;

    __asm__ volatile("invlpg (%0)" : : "r"(virtual_address) : "memory");

    return physical_address;
}
