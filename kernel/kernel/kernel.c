#include <stdio.h>

#include <gdt/gdt.h>
#include <idt/idt.h>
#include <kernel/tty.h>

void kernel_main(void)
{
	gdt_init(); // Initialises the GDT
	idt_init(); // Initialises the IDT
	// TODO: Introduce interrupt handling
	terminal_init(); // Initialises the terminal emulator

	printf("Welcome to Newtonix!\n");
}