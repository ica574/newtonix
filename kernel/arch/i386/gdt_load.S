# Loads the GDT and reloads the segment registers

.globl gdt_load # Makes the gdt_load function visible to C
gdt_load:
	pushl %ebp
	movl %esp, %ebp
	
	movl 8(%ebp), %eax
	lgdt (%eax)

	movzwl 12(%ebp), %eax
	pushl %eax

	pushl $.reload_cs
	lret

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

.section .note.GNU-stack,"",@progbits
