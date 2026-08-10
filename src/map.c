#include "types.h"

#define PAGE_PRESENT 1
#define PAGE_WRITE 2
#define PAGE_HUGE 0x80

#define PAGE_2M 0x200000LLU

#define PDPT_COUNT 512
#define PD_COUNT 512

struct multiboot_mmap_entry
{
	u32 size;
	u64 addr;
	u64 len;
	u32 type;
} __attribute__((packed));

struct multiboot_info
{
	u32 flags;
	u32 mem_lower;
	u32 mem_upper;

	u32 boot_device;
	u32 cmdline;

	u32 mods_count;
	u32 mods_addr;

	u8 syms[16];

	u32 mmap_length;
	u32 mmap_addr;
} __attribute__((packed));

extern u64 pml4[], pdpt[], page_tables[];

static void zero_page(void *ptr);
static void map_2m(u64 addr);

void setup_paging(struct multiboot_info *mb_info)
{
	u32 i;

	zero_page(pml4);
	zero_page(pdpt);

	for (i = 0; i < 512; ++i)
	{
		zero_page(page_tables + i * 512);
	}

	pml4[0] = (u64)pdpt | PAGE_PRESENT | PAGE_WRITE;

	i = 0;

	while (i < mb_info->mmap_length)
	{
		struct multiboot_mmap_entry *entry;

		entry = (struct multiboot_mmap_entry *)(mb_info->mmap_addr + i);

		if (entry->type == 1)
		{
			u64 j, end;
			j = (entry->addr + PAGE_2M - 1) & ~(PAGE_2M - 1);

			end = (entry->addr + entry->len) & ~(PAGE_2M - 1);

			while (j < end)
			{

				map_2m(j);
				j += PAGE_2M;
			}
		}
		i += entry->size + sizeof entry->size;
	}
}

static void zero_page(void *ptr)
{
	int i;
	for (i = 0; i < 512; ++i)
	{
		((u64 *)ptr)[i] = 0;
	}
}

static void map_2m(u64 addr)
{
	u64 pd_idx, pdpt_idx;
	u64 *pd;

	pd_idx = (addr >> 21) & 0x1FF;
	pdpt_idx = (addr >> 30) & 0x1FF;

	pd = page_tables + pdpt_idx * 512;

	if (!(pdpt[pdpt_idx] & PAGE_PRESENT))
	{
		pdpt[pdpt_idx] = (u64)pd | PAGE_PRESENT | PAGE_WRITE;
	}

	pd[pd_idx] = addr | PAGE_PRESENT | PAGE_WRITE | PAGE_HUGE;
}
