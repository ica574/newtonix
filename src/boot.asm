; boot.asm v0.1
; Multiboot header for bootloader to identify kernel.

MBALIGN  equ  1 << 0            				; align loaded modules on page boundaries!
MEMINFO  equ  1 << 1            				; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO					; this is the Multiboot 'flag' field!
MAGIC    equ  0x1BADB002						; 'Magic number' for bootloader to detect kernel header
CHECKSUM equ -(MAGIC + MBFLAGS)					; Checksum used to prove that OS is multiboot

section .multiboot								; Multiboot header, searched for by bootloader in the first 8 KiB of the kernel for ID
align 4											; Aligned at a 32-bit boundary for speed/processor requirements
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .bss									; Uninitialised data
align 16										; Aligns upcoming stack at 16 bytes according to System V ABI
stack_bottom:									; Defines the beginning of the stack
resb 16384										; Allocates the next 16384 bytes
stack_top:										; Ends the stack after allocation
; Stack grows downwards in x86, hence its definition from bottom to top

section .text									; Processor instructions
global _start:function (_start.end - _start)	; Makes label visible to the linker, calculates size of _start
_start:
	mov esp, stack_top							; Configures stack for C, points esp to stack_top
	
	; Kernel has full control from here

	; This is a good place to initialize crucial processor state before the
	; high-level kernel is entered. It's best to minimize the early
	; environment where crucial features are offline. Note that the
	; processor is not fully initialized yet: Features such as floating
	; point instructions and instruction set extensions are not initialized
	; yet. The GDT should be loaded here. Paging should be enabled here.
	; C++ features such as global constructors and exceptions will require
	; runtime support to work as well.

	extern kernel_main							; Declares existence of external kernel function
	call kernel_main							; Calls main kernel function

	cli											; Disable interrupts
.hang:	hlt										; Wait for next interrupt, computer locked since disabled previously
	jmp .hang									; Jump to hlt, only for NMI
.end:											; End of program