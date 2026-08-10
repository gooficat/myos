#include "stub.h"
#include <stdint.h>

__attribute__((
	section(".multiboot2"), used)) struct multiboot2_header multiboot_header = {
	.magic = MULTIBOOT2_MAGIC,
	.architecture = MULTIBOOT2_ARCH_I386,
	.header_length = MULTIBOOT2_HEADER_LENGTH,
	.checksum = MULTIBOOT2_CHECKSUM,
	.end_tag = {.type = 0, .flags = 0, .size = 8},
};
