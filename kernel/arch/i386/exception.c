#include <stdio.h>

#include <kernel/panic.h>
#include <idt/interrupt.h>

static const char *exception_names[32] = {
    "Divide Error",                         // 0
    "Debug",                                // 1
    "Non-maskable Interrupt",               // 2
    "Breakpoint",                           // 3
    "Overflow",                             // 4
    "Bound Range Exceeded",                 // 5
    "Invalid Opcode",                       // 6
    "Device Not Available",                 // 7
    "Double Fault",                         // 8
    "Coprocessor Segment Overrun",          // 9
    "Invalid TSS",                          // 10
    "Segment Not Present",                  // 11
    "Stack-Segment Fault",                  // 12
    "General Protection Fault",             // 13
    "Page Fault",                           // 14
    "Reserved",                             // 15
    "x87 Floating-Point Exception",         // 16
    "Alignment Check",                      // 17
    "Machine Check",                        // 18
    "SIMD Floating-Point Exception",        // 19
    "Virtualization Exception",             // 20
    "Reserved",                             // 21
    "Reserved",                             // 22
    "Reserved",                             // 23
    "Reserved",                             // 24
    "Reserved",                             // 25
    "Reserved",                             // 26
    "Reserved",                             // 27
    "Reserved",                             // 28
    "Reserved",                             // 29
    "Reserved",                             // 30
    "Reserved",                             // 31
};

__attribute__((__noreturn__))
void exception_handler(const interrupt_frame *frame)
{
    (void)frame;

    if (frame->vector < 32) {
        printf("CPU exception %s\n", exception_names[frame->vector]);
    } else {
        printf("Unknown CPU exception\n");
    }

    printf("Vector: 0x%x\n", (unsigned int) frame->vector);
    printf("Error:  0x%x\n", (unsigned int) frame->error_code);
    printf("EIP:    0x%x\n", (unsigned int) frame->eip);
    printf("CS:     0x%x\n", (unsigned int) frame->cs);
    printf("EFLAGS: 0x%x\n", (unsigned int) frame->eflags);

    kernel_panic("Unhandled CPU exception");
}
