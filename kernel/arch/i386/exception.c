#include <stdio.h>

#include <kernel/panic.h>
#include <idt/interrupt.h>

__attribute__((__noreturn__))
void breakpoint_exception_handler(const interrupt_frame *frame)
{
    (void)frame;

    printf("Breakpoint exception\n");
    printf("Vector: 0x%x\n", (unsigned int) frame->vector);
    printf("Error:  0x%x\n", (unsigned int) frame->error_code);
    printf("EIP:    0x%x\n", (unsigned int) frame->eip);
    printf("CS:     0x%x\n", (unsigned int) frame->cs);
    printf("EFLAGS: 0x%x\n", (unsigned int) frame->eflags);

    kernel_panic("Unhandled CPU exception");
}
