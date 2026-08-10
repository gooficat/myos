.code32
.globl _start
.type _start, @function
.section .text

_start:
	cli

	lgdt gdt_desc

	mov $pml4, %eax
	mov %eax, %cr3

	mov %cr4, %eax
	or $0x20, %eax
	mov %eax, %cr4

	mov $0xC0000080, %ecx
	rdmsr
	or $0x100, %eax
	wrmsr

	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0

	ljmp $0x08, $long_mode


.code64

long_mode:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss

	mov $stack_top, %rsp

	call kernel_main

halt:
	hlt
	jmp halt

.section .rodata

gdt:
	.quad 0
	.quad 0x00AF9A000000FFFF
	.quad 0x00CF92000000FFFF
gdt_end:

gdt_desc:
	.word gdt_end - gdt - 1
	.long gdt
	.word 0

.section .data

.align 4096
pml4:
	.quad pdpt + 0x003
.align 4096
pdpt:
	.quad pd + 0x3
.align 4096
pd:
	.quad 0x83
	.fill 511, 8, 0


.section .bss

.align 16
stack:
	.skip 16384

stack_top:
