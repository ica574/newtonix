.section .text
.extern breakpoint_exception_handler
.global breakpoint_isr
.type breakpoint_isr, @function

breakpoint_isr:
    cld # Clear direction flag
    pushal

    movl %esp, %eax
    andl $-16, %esp # Align stack by rounding down to 16-byte boundary

    subl $12, %esp
    pushl %eax

    call breakpoint_exception_handler # Call C handler

    movl (%esp), %esp
    popal
    iret

.size breakpoint_isr, . - breakpoint_isr
.section .note.GNU-stack,"",@progbits
