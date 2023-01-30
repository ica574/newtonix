# Loads the GDT into the S registers

.globl gdt_load # Makes the gdt_load function visible to C
gdt_load:
	pushl %ebp
	movl %esp, %ebp
	
	movl 8(%ebp), %eax
	lgdt (%eax)

	movl 12(%ebp), %eax
	pushl %eax
	call .reload_cs
	ret

.reload_cs:
	movw 16(%ebp), %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	movl %ebp, %esp
	popl %ebp
	ret