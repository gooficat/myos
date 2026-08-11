#include "vga.h"
#include "types.h"

void kernel_main(void)
{
  static char const hello[] = "Hello, World";
  VGA_clear();
  VGA_put_str(hello, 10, 10);
}
