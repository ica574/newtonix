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
ISR_ERROR       15
ISR_ERROR       16
ISR_ERROR       17

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

.section .note.GNU-stack,"",@progbits
