#include "asm.h"
#include <kernel/memlayout.h>

# 声明multiboot用到的常量
.set ALIGN,    1<<0             # flags第0位，指示是否页对齐
.set MEMINFO,  1<<1             # flags第1位，指示是否提供内存映射信息
.set FLAGS,    ALIGN | MEMINFO  # multiboot的flags域内容
.set MAGIC,    0x1BADB002       # multiboot的magic域
.set CHECKSUM, -(MAGIC + FLAGS) # multiboot的checksum域

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 4096
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
.quad 0
glb_mboot_ptr:
.quad 0

# The kernel entry point.
.section .text
.global _start
.global glb_mboot_ptr
.type _start, @function
_start:

	lgdt V2P(__gdtdesc)
	ljmp $0x08, $next
next:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss	

	movl %ebx, %eax
	addl $KERNBASE, %eax

	movl %eax, glb_mboot_ptr
	movl $stack_top, %esp

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b


.align 8
__gdt:
	SEG_NULL
	SEG_ASM(STA_X | STA_R, 0x0 - KERNBASE, 0xffffffff)
	SEG_ASM(STA_W, 0x0 - KERNBASE, 0xffffffff)
__gdtdesc:
	.word (. - __gdt - 1)
	.long V2P(__gdt)

.size _start, . - _start 