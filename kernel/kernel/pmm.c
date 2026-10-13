#include <stdint.h>
#include <string.h>

#include <kernel/pmm.h>
#include <kernel/panic.h>

#define PHYSICAL_PAGE_COUNT (1U << 20)
#define BITMAP_SIZE (PHYSICAL_PAGE_COUNT / 8U)
#define PHYSICAL_ADDRESS_LIMIT (1ULL << 32)

static uint8_t page_bitmap[BITMAP_SIZE];

// Helper functions

static void mark_page_used(uint32_t page)
{
    uint32_t byte_index = page / 8;
    uint32_t bit_index = page % 8;

    page_bitmap[byte_index] |= (1U << bit_index);
}

static void mark_page_free(uint32_t page)
{
    uint32_t byte_index = page / 8;
    uint32_t bit_index = page % 8;

    page_bitmap[byte_index] &= ~(1U << bit_index);
}

static int is_page_used(uint32_t page)
{
    uint32_t byte_index = page / 8U;
    uint32_t bit_index = page % 8U;

    return (page_bitmap[byte_index] & (1U << bit_index)) != 0;
}

// Main functions

void pmm_initialize(void)
{
    memset(page_bitmap, 0xFF, sizeof(page_bitmap));
}

void pmm_add_usable_region(uint64_t address, uint64_t length)
{
    // An empty region contributes no pages

    if (length == 0)
    {
        return;
    }

    // Addresses above 4 GiB cannot be managed due to the 32-bit limit

    if (address >= PHYSICAL_ADDRESS_LIMIT)
    {
        return;
    }

    uint64_t region_end;

    // Clamp the region to the 32-bit physical address limit

    if (length > PHYSICAL_ADDRESS_LIMIT - address)
    {
        region_end = PHYSICAL_ADDRESS_LIMIT;
    } else {
        region_end = address + length;
    }

    uint64_t first_page = (address + PAGE_SIZE - 1U) / PAGE_SIZE;

    uint64_t end_page = region_end / PAGE_SIZE;

    for (uint64_t page = first_page; page < end_page; page++)
    {
        mark_page_free((uint32_t)page);
    }
}

void pmm_reserve_region(uint64_t address, uint64_t length)
{
    // An empty region contributes no pages

    if (length == 0)
    {
        return;
    }

    // Addresses above 4 GiB cannot be managed due to the 32-bit limit

    if (address >= PHYSICAL_ADDRESS_LIMIT)
    {
        return;
    }

    uint64_t region_end;

    if (length > PHYSICAL_ADDRESS_LIMIT - address)
    {
        region_end = PHYSICAL_ADDRESS_LIMIT;
    } else {
        region_end = address + length;
    }

    uint64_t first_page = address / PAGE_SIZE;

    uint64_t end_page = region_end / PAGE_SIZE;

    if ((region_end % PAGE_SIZE) != 0)
    {
        end_page++;
    }

    for (uint64_t page = first_page; page < end_page; page++)
    {
        mark_page_used((uint32_t)page);
    }
}

uint32_t pmm_count_free_pages(void)
{
    uint32_t free_pages = 0;

    for (uint32_t page = 0; page < PHYSICAL_PAGE_COUNT; page++)
    {
        if (!is_page_used(page))
        {
            free_pages++;
        }
    }

    return free_pages;
}
