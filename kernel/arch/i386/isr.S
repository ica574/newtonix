.section .text
.extern breakpoint_exception_handler
.global breakpoint_isr
.type breakpoint_isr, @function

breakpoint_isr:
    cld # Clear direction flag

    andl $-16, %esp # Align stack by rounding down to 16-byte boundary

    call breakpoint_exception_handler # Call C handler

    cli
1:
    hlt
    jmp 1b

.size breakpoint_isr, . - breakpoint_isr
.section .note.GNU-stack,"",@progbits
