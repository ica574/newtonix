#include <stdint.h>
#include <stdio.h>

#include <gdt/gdt.h>
#include <idt/idt.h>
#include <kernel/multiboot.h>
#include <kernel/paging.h>
#include <kernel/tty.h>

void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info_address)
{
    gdt_init(); // Initialises the GDT
    idt_init(); // Initialises the IDT

    terminal_init(); // Initialises the terminal emulator

    printf("Welcome to Newtonix!\n");

    multiboot_initialize(multiboot_magic, multiboot_info_address);

    paging_initialise();

    printf("Paging structures initialised.\n");
    printf("Paging enabled.\n");

    printf("Kernel initialisation complete!\n");

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}
