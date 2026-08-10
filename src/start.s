.code32
.globl _start
.type _start, @function

.extern setup_paging
.extern kernel_main

.section .text

_start:
	cli
	mov %ebx, %esi

	lgdt gdt_desc

	push %esi
	call setup_paging
	add $4, %esp

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

.section .bss

.align 4096
.globl pml4
pml4:
	.skip 4096

.align 4096
.globl pdpt
pdpt:
	.skip 4096

.align 4096
.globl page_tables
page_tables:
	.skip 4096 * 512

.align 16
stack:
	.skip 16384

stack_top:
