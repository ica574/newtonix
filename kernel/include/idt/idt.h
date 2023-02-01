#pragma once
#include <stdint.h>

void idt_init();
void idt_disablegate(int interrupt);
void idt_enablegate(int interrupt);
void idt_setgate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags);