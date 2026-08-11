#include "types.h"

static u16 *VGA = (u16 *)0xB8000;
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void VGA_write_char(char ch, u8 flags, u32 x, u32 y)
{
	VGA[y * VGA_WIDTH + x] = flags << 8 | ch;
}

int VGA_put_str(char const *str, u32 start_x, u32 start_y)
{
	u32 x, y;
	if (start_x > VGA_WIDTH || start_y > VGA_HEIGHT)
	{
		return 0;
	}

	x = start_x;
	y = start_y;

	while (*str)
	{
		if (*str == '\n')
		{
			++y;
		}
		else if (*str == '\r')
		{
			x = start_x;
		}
		else
		{
			VGA_write_char(*str, 0b111, x, y);
			if (++x > VGA_WIDTH)
			{
				++y;
				x = 0;
			}
		}
		++str;
	}
}

void VGA_clear(void)
{
	for (u64 i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
	{
		VGA[i] = 0;
	}
}
