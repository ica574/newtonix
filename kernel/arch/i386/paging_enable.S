.section .text
.global paging_enable
.type paging_enable, @function

paging_enable:
    # Under 32-bit cdecl, the first argument is 4 bytes above ESP
    movl 4(%esp), %eax

    # CR3 holds the physical address of the page directory
    movl %eax, %cr3

    # Enable paging by setting bit 31 of CR0
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    ret

.size paging_enable, . - paging_enable
.section .note.GNU-stack,"",@progbits
