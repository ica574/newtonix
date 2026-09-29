#include <stdio.h>
#include <stddef.h>

#include <kernel/panic.h>

__attribute__((__noreturn__))
void kernel_panic(const char *message) 
{
    // Disable maskable interrupts
    __asm__ volatile ("cli");

    // Add failsafe for null messages
    if (message == NULL)
    {
        message = "Unknown fatal error";
    }

    // Report the fatal error
    printf("\nKERNEL PANIC: %s\n", message);

    // Halt again if a non-maskable event wakes the CPU
    for (;;) 
    {
        __asm__ volatile ("hlt");
    }
}
