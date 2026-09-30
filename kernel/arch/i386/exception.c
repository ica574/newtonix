#include <kernel/panic.h>

__attribute__((__noreturn__))
void breakpoint_exception_handler(void)
{
    kernel_panic("Breakpoint exception");
}
