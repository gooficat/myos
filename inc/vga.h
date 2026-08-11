#ifndef __VGA__H__
#define __VGA__H__

#include "types.h"

extern u16 *VGA;
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void VGA_write_char(char ch, u8 flags, u32 x, u32 y);

int VGA_put_str(char const *str, u32 start_x, u32 start_y);
void VGA_clear(void);

#endif //!__VGA__H__
