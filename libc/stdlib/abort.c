#include <stdio.h>
#include <stdlib.h>

// Include kernel panic headers if in the kernel
#if defined(__is_libk)
#include <kernel/panic.h>
#endif

__attribute__((__noreturn__))
void abort(void) 
{
#if defined(__is_libk)
    // A kernel abort is fatal to the entire system
    kernel_panic("abort()");
#else
    // Future userspace abort function
    // TODO: Abnormally terminate the process as if by SIGABRT.
    printf("abort()\n");
    for (;;)
    {
    }
#endif
}
