#ifndef NEWTONIX_PANIC_H
#define NEWTONIX_PANIC_H


__attribute__((__noreturn__))
void kernel_panic(const char *message);

#endif
