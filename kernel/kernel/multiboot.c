#include <stdint.h>
#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/panic.h>
#include <kernel/pmm.h>
#include <multiboot/multiboot.h>

extern char __kernel_start[];
extern char __kernel_end[];

void multiboot_initialize(uint32_t multiboot_magic, uint32_t multiboot_info_address)
{
    if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        kernel_panic("Invalid Multiboot bootloader magic");
    }

    const multiboot_info *boot_info = (const multiboot_info *)multiboot_info_address;

    printf("Multiboot flags: 0x%x\n", (unsigned int)boot_info->flags);

    if ((boot_info->flags & MULTIBOOT_INFO_MEMORY_MAP) == 0)
    {
        kernel_panic("Multiboot memory map unavailable");
    }

    pmm_initialize();

    printf("Memory map address: 0x%x\n", (unsigned int)boot_info->mmap_addr);
    printf("Memory map length: 0x%x\n", (unsigned int)boot_info->mmap_length);

    printf("Multiboot magic: 0x%x\n", (unsigned int)multiboot_magic);
    printf("Multiboot info: 0x%x\n", (unsigned int)multiboot_info_address);

    uint32_t offset = 0;

    while (offset < boot_info->mmap_length)
    {
        if (boot_info->mmap_length - offset < sizeof(uint32_t))
        {
            kernel_panic("Truncated Multiboot memory-map entry");
        }

        const multiboot_memory_map_entry *entry =
            (const multiboot_memory_map_entry *)(boot_info->mmap_addr + offset);

        if (entry->size < 20)
        {
            kernel_panic("Malformed Multiboot memory-map entry");
        }

        uint32_t remaining = boot_info->mmap_length - offset;

        if (entry->size > remaining - sizeof(entry->size))
        {
            kernel_panic("Multiboot memory-map entry exceeds map");
        }

        uint32_t entry_length = entry->size + sizeof(entry->size);

        uint32_t address_high = (uint32_t)(entry->address >> 32);
        uint32_t address_low = (uint32_t)entry->address;

        uint32_t length_high = (uint32_t)(entry->length >> 32);
        uint32_t length_low = (uint32_t)entry->length;

        printf("Region:\n");
        printf("  address high: 0x%x\n", address_high);
        printf("  address low:  0x%x\n", address_low);
        printf("  length high:  0x%x\n", length_high);
        printf("  length low:   0x%x\n", length_low);
        printf("  type:         0x%x\n", entry->type);

        offset += entry_length;

        if (entry->type == 1)
        {
            pmm_add_usable_region(entry->address, entry->length);
        }
    }

    if (offset != boot_info->mmap_length)
    {
        kernel_panic("Malformed Multiboot memory map");
    }

    pmm_reserve_region(0, 0x100000);

    uint64_t kernel_start = (uint32_t)__kernel_start;
    uint64_t kernel_end = (uint32_t)__kernel_end;

    pmm_reserve_region(kernel_start, kernel_end - kernel_start);

    printf("Free physical pages: %x\n", (unsigned int)pmm_count_free_pages());
}
