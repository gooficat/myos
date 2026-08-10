#include "types.h"

#define PAGE_PRESENT 1
#define PAGE_WRITE 2
#define PAGE_HUGE 0x80

#define PAGE_2M 0x200000ULL

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36D76289

#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_MMAP 6

#define MULTIBOOT_MEMORY_AVAILABLE 1

struct multiboot2_tag
{
	u32 type;
	u32 size;
};

struct multiboot2_tag_mmap
{
	u32 type;
	u32 size;
	u32 entry_size;
	u32 entry_version;

	u8 entries[];
};

struct multiboot2_mmap_entry
{
	u64 addr;
	u64 len;
	u32 type;
	u32 reserved;
};

static void zero_page(void *page);

extern u64 pml4[], pdpt[], page_tables[];

static void zero_page(void *ptr);
static void map_2m(u64 addr);

void setup_paging(void *multiboot_info)
{
	struct multiboot2_tag *tag;
	u32 i;

	/* Keep the active identity mappings until the replacement tables are ready. */
	for (i = 1; i < 512; ++i)
	{
		zero_page(page_tables + i * 512);
	}

	for (i = 1; i < 512; ++i)
	{
		pdpt[i] = 0;
	}

	map_2m(0);
	map_2m(0x200000);

	i = 8;

	for (;;)
	{
		struct multiboot2_tag *tag;
		tag = (struct multiboot2_tag *)((u8 *)multiboot_info + i);

		if (tag->type == MULTIBOOT_TAG_TYPE_END)
		{
			break;
		}
		else if (tag->type == MULTIBOOT_TAG_TYPE_MMAP)
		{
			struct multiboot2_tag_mmap *mmap;
			u32 entry_offset;

			mmap = (struct multiboot2_tag_mmap *)tag;

			entry_offset = 0;

			while (entry_offset + mmap->entry_size <= mmap->size - 16)
			{
				struct multiboot2_mmap_entry *entry;

				entry = (struct multiboot2_mmap_entry *)(mmap->entries + entry_offset);

				if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
				{
					u64 start, end;

					start = (entry->addr + PAGE_2M - 1) & ~(PAGE_2M - 1);

					end = (entry->addr + entry->len) & ~(PAGE_2M - 1);

					while (start < end)
					{
						map_2m(start);
						start += PAGE_2M;
					}
				}

				entry_offset += mmap->entry_size;
			}
		}

		i += (tag->size + 7) & ~7;
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
	u64 pd_idx, pdpt_idx, *pd;

	pd_idx = (addr >> 21) & 0x1FF;
	pdpt_idx = (addr >> 30) & 0x1FF;

	pd = page_tables + pdpt_idx * 512;

	if (!(pdpt[pdpt_idx] & PAGE_PRESENT))
	{
		pdpt[pdpt_idx] = (u64)pd | PAGE_PRESENT | PAGE_WRITE;
	}

	pd[pd_idx] = addr | PAGE_PRESENT | PAGE_WRITE | PAGE_HUGE;
}
