#include <stdio.h>
#include <stdint.h>

#include <gdt/gdt.h>
#include <idt/idt.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>

void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info_address)
{
	gdt_init(); // Initialises the GDT
	idt_init(); // Initialises the IDT

    terminal_init(); // Initialises the terminal emulator

	printf("Welcome to Newtonix!\n");

    multiboot_initialize(
            multiboot_magic,
            multiboot_info_address
    );
}
