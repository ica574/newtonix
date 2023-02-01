# Loads the IDT

.globl idt_load
idt_load:
    pushl %ebp
    movl %esp, %ebp

    movl 8(%ebp), %eax
    lidt (%eax) # Loads IDT

    movl %ebp, %esp
    popl %ebp
    ret