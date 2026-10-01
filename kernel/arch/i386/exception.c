#include <kernel/panic.h>
#include <idt/interrupt.h>

__attribute__((__noreturn__))
void breakpoint_exception_handler(const interrupt_frame *frame)
{
    (void)frame;
    kernel_panic("Breakpoint exception");
}
