#ifndef __STUB__H__
#define __STUB__H__

#include "types.h"

struct multiboot2_header
{
  u32 magic;
  u32 architecture;
  u32 header_length;
  u32 checksum;
  struct
  {
    u16 type;
    u16 flags;
    u32 size;
  } end_tag;
};

#define MULTIBOOT2_MAGIC UINT32_C(0xE85250D6)
#define MULTIBOOT2_ARCH_I386 UINT32_C(0)
#define MULTIBOOT2_HEADER_LENGTH ((u32)sizeof(struct multiboot2_header))
#define MULTIBOOT2_CHECKSUM \
  ((u32)(-(MULTIBOOT2_MAGIC + MULTIBOOT2_ARCH_I386 + MULTIBOOT2_HEADER_LENGTH)))

#endif
