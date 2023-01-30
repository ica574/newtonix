#include <stdio.h>

#include <gdt/gdt.h>
#include <kernel/tty.h>

void kernel_main(void)
{
	gdt_init();		 // Initialises the GDT
	terminal_init(); // Initialises the terminal emulator

	printf("Welcome to Newtonix!\n");
}