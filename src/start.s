.code32
.globl _start
.type _start, @function

.extern setup_paging
.extern kernel_main

.section .text

_start:
	cli # disable interrupts

	cmp $0x36d76289, %eax # check magic
	jne halt

	mov %ebx, %esi # store pointer to multiboot2 records

	lgdt gdt_desc # load global descriptor table

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

	mov %esi, %edi
	call setup_paging # set up the real tables	
	mov $pml4, %rax
	mov %rax, %cr3


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
.globl pml4
pml4:
	.quad pdpt + 3
	.fill 511, 8, 0
.align 4096
.globl pdpt
pdpt:
	.quad page_tables + 3
	.fill 511, 8, 0

.align 4096
.globl page_tables
page_tables:
	.quad 0x83
	.quad 0x200083
	.fill 510, 8, 0
	.skip 4096 * 511

.section .bss

.align 16
stack:
	.skip 16384

stack_top:
