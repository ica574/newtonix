/*
 * Refer to Intel Software Developer's Manual, Volume 3A
 */

#include <stdint.h>

#include <idt/idt.h>

// Helper macros
#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)

typedef struct
{
    uint16_t BaseLow;
    uint16_t SegmentSelector;
    uint8_t Reserved;
    uint8_t Flags;
    uint16_t BaseHigh;
} __attribute__((packed)) segment_descriptor; // Describes an IDT entry

typedef struct
{
    uint16_t Limit;
    segment_descriptor *Ptr;
} __attribute__((packed)) idt_descriptor; // Defines the IDT

typedef enum
{
    IDT_FLAG_GATE_TASK = 0x5,
    IDT_FLAG_GATE_16BIT_INT = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP = 0x7,
    IDT_FLAG_GATE_32BIT_INT = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP = 0xF,

    IDT_FLAG_RING0 = (0 << 5),
    IDT_FLAG_RING1 = (1 << 5),
    IDT_FLAG_RING2 = (2 << 5),
    IDT_FLAG_RING3 = (3 << 5),

    IDT_FLAG_PRESENT = 0x80,

} IDT_FLAGS;

segment_descriptor system_idt[256]; // The IDT, with the 256 interrupts

idt_descriptor system_idt_descriptor = {sizeof(system_idt) - 1, system_idt};

void __attribute__((cdecl)) idt_load(idt_descriptor *idtDescriptor);

void idt_setgate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags)
{
    system_idt[interrupt].BaseLow = ((uint32_t)base) & 0xFFFF;
    system_idt[interrupt].SegmentSelector = segmentDescriptor;
    system_idt[interrupt].Reserved = 0;
    system_idt[interrupt].Flags = flags;
    system_idt[interrupt].BaseHigh = ((uint32_t)base >> 16) & 0xFFFF;
}

void idt_enablegate(int interrupt)
{
    FLAG_SET(system_idt[interrupt].Flags, IDT_FLAG_PRESENT);
}

void idt_disablegate(int interrupt)
{
    FLAG_UNSET(system_idt[interrupt].Flags, IDT_FLAG_PRESENT);
}

void idt_init()
{
    idt_load(&system_idt_descriptor);
}