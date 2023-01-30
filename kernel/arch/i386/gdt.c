/*
 * Refer to Intel Software Developer's Manual, Volume 3A
 */

#include <stdint.h>

#include <gdt/gdt.h>

typedef struct
{
    uint16_t LimitLow;                        // limit (bits 0-15)
    uint16_t BaseLow;                         // base (bits 0-15)
    uint8_t BaseMiddle;                       // base (bits 16-23)
    uint8_t Access;                           // access
    uint8_t FlagsLimitHi;                     // limit (bits 16-19) | flags
    uint8_t BaseHigh;                         // base (bits 24-31)
} __attribute__((packed)) segment_descriptor; // Describes a GDT entry

typedef struct
{
    uint16_t Limit;                       // sizeof(gdt) - 1
    segment_descriptor *Ptr;              // address of GDT
} __attribute__((packed)) gdt_descriptor; // Defines the GDT itself

typedef enum
{
    GDT_ACCESS_CODE_READABLE = 0x02,
    GDT_ACCESS_DATA_WRITEABLE = 0x02,

    GDT_ACCESS_CODE_CONFORMING = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN = 0x04,

    GDT_ACCESS_DATA_SEGMENT = 0x10,
    GDT_ACCESS_CODE_SEGMENT = 0x18,

    GDT_ACCESS_DESCRIPTOR_TSS = 0x00,

    GDT_ACCESS_RING0 = 0x00,
    GDT_ACCESS_RING1 = 0x20,
    GDT_ACCESS_RING2 = 0x40,
    GDT_ACCESS_RING3 = 0x60,

    GDT_ACCESS_PRESENT = 0x80,

} GDT_ACCESS; // Access control markers for segment descriptors

typedef enum
{
    GDT_FLAG_64BIT = 0x20,
    GDT_FLAG_32BIT = 0x40,
    GDT_FLAG_16BIT = 0x00,

    GDT_FLAG_GRANULARITY_1B = 0x00,
    GDT_FLAG_GRANULARITY_4K = 0x80,
} GDT_FLAGS; // Segment descriptor properties

// Helper macros
#define GDT_LIMIT_LOW(limit) (limit & 0xFFFF)
#define GDT_BASE_LOW(base) (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base) ((base >> 16) & 0xFF)
#define GDT_FLAGS_LIMIT_HI(limit, flags) (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base) ((base >> 24) & 0xFF)

#define SEGMENT_DESCRIPTOR(base, limit, access, flags) \
    {                                                  \
        GDT_LIMIT_LOW(limit),                          \
            GDT_BASE_LOW(base),                        \
            GDT_BASE_MIDDLE(base),                     \
            access,                                    \
            GDT_FLAGS_LIMIT_HI(limit, flags),          \
            GDT_BASE_HIGH(base)                        \
    } // Splits attributes per the x86 definition of a segment descriptor

segment_descriptor system_gdt[] = {
    // NULL descriptor - required for flat model in protected mode
    SEGMENT_DESCRIPTOR(0, 0, 0, 0),

    // Kernel 32-bit code segment
    SEGMENT_DESCRIPTOR(0,
                       0xFFFFF,
                       GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
                       GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K),

    // Kernel 32-bit data segment
    SEGMENT_DESCRIPTOR(0,
                       0xFFFFF,
                       GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
                       GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K),

};

gdt_descriptor system_gdt_descriptor = {sizeof(system_gdt) - 1, system_gdt};

void __attribute__((cdecl)) gdt_load(gdt_descriptor *descriptor, uint16_t code_segment, uint16_t data_segment);

void gdt_init()
{
    gdt_load(&system_gdt_descriptor, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}