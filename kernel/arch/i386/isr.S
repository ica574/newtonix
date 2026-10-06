.section .text
.extern exception_handler

.macro ISR_NO_ERROR vector
.global isr_\vector
.type isr_\vector, @function
isr_\vector:
    pushl $0           # Synthetic error code
    pushl $\vector     # Exception vector
    jmp isr_common
.endm

.macro ISR_ERROR vector
.global isr_\vector
.type isr_\vector, @function
isr_\vector:
    pushl $\vector     # Exception vector only needed
    jmp isr_common
.endm


ISR_NO_ERROR    0
ISR_NO_ERROR    1
ISR_NO_ERROR    2
ISR_NO_ERROR    3
ISR_NO_ERROR    4
ISR_NO_ERROR    5
ISR_NO_ERROR    6
ISR_NO_ERROR    7
ISR_ERROR       8
ISR_NO_ERROR    9
ISR_ERROR       10
ISR_ERROR       11
ISR_ERROR       12
ISR_ERROR       13
ISR_ERROR       14
ISR_NO_ERROR    15
ISR_NO_ERROR    16
ISR_ERROR       17
ISR_NO_ERROR    18
ISR_NO_ERROR    19
ISR_NO_ERROR    20
ISR_NO_ERROR    21
ISR_NO_ERROR    22
ISR_NO_ERROR    23
ISR_NO_ERROR    24
ISR_NO_ERROR    25
ISR_NO_ERROR    26
ISR_NO_ERROR    27
ISR_NO_ERROR    28
ISR_NO_ERROR    29
ISR_NO_ERROR    30
ISR_NO_ERROR    31

isr_common:
    cld
    pushal

    # Save the address of the interrupt frame.
    movl %esp, %eax

    # Create an aligned stack for the C function call.
    andl $-16, %esp
    subl $12, %esp
    pushl %eax

    call exception_handler

    # Restore the original interrupt stack.
    movl (%esp), %esp
    popal

    # Remove vector and error code.
    addl $8, %esp

    # Restore EIP, CS, and EFLAGS.
    iret

.section .rodata
.align 4

.global isr_stub_table
.type isr_stub_table, @object

isr_stub_table:
    .long isr_0
    .long isr_1
    .long isr_2
    .long isr_3
    .long isr_4
    .long isr_5
    .long isr_6
    .long isr_7
    .long isr_8
    .long isr_9
    .long isr_10
    .long isr_11
    .long isr_12
    .long isr_13
    .long isr_14
    .long isr_15
    .long isr_16
    .long isr_17
    .long isr_18
    .long isr_19
    .long isr_20
    .long isr_21
    .long isr_22
    .long isr_23
    .long isr_24
    .long isr_25
    .long isr_26
    .long isr_27
    .long isr_28
    .long isr_29
    .long isr_30
    .long isr_31

.size isr_stub_table, . - isr_stub_table

.section .note.GNU-stack,"",@progbits
