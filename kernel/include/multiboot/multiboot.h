#ifndef MULTIBOOT_MULTIBOOT_H
#define MULTIBOOT_MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002
#define MULTIBOOT_INFO_MEMORY_MAP   (1U << 6)

typedef struct
{
    uint32_t flags;

    uint32_t mem_lower;
    uint32_t mem_upper;

    uint32_t boot_device;
    uint32_t cmdline;

    uint32_t mods_count;
    uint32_t mods_addr;

    uint32_t symbols[4];

    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed)) multiboot_info;

typedef struct
{
    uint32_t size;
    uint64_t address;
    uint64_t length;
    uint32_t type;

} __attribute__((packed)) multiboot_memory_map_entry;

#endif
