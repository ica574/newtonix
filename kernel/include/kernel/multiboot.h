#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

#include <stdint.h>


void multiboot_initialize(
        uint32_t magic,
        uint32_t information_address
);

#endif
