#include "../../head/c/paging.h"
#include "../../head/c/asmutils.h"

page_state physical_state[1024*1024] = {};
void basic_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries)
{
	setup_page_directory(pageDirectory, entries);
	for (uint i = 0; i < 1024; i++)
	{
		for (uint j = 0; j < 1024; j++)
		{
			void* addr =(void*) ((i * 1024 + j) * 4 * 1024);
			k_mmap(entries, addr,addr, in_use);
		}	
	}
	load_page_directory(pageDirectory);
}


void k_mmap(PageTableEntry* entries, void* virtual, void* physical, page_state destination_state)
{
	uint virtual_index  = ((uint) virtual ) >> 12;
	uint physical_index = ((uint) physical) >> 12; 
	physical_state [physical_index] = destination_state;
	entries[virtual_index].user_supervision     = 1;
	entries[virtual_index].readwrite            = 1;
	entries[virtual_index].PWT                  = 0;
	entries[virtual_index].present              = 1;
	entries[virtual_index].page_attribute_table = 0;
	entries[virtual_index].is_cache_disabled    = 0;
	entries[virtual_index].global               = 0;
	entries[virtual_index].address_low          = physical_index % 16;
	entries[virtual_index].address_high         = physical_index>> 4;
}


void setup_page_directory(PageDirectoryEntry* pageDirectory, PageTableEntry* entries)
{
	for (int i = 0; i < 1024; i++)
	{
		pageDirectory[i].present           = 1;
		pageDirectory[i].is_big            = 0;
		pageDirectory[i].PWT               = 0;
		pageDirectory[i].readwrite         = 1;
		pageDirectory[i].user_supervision  = 1;
		pageDirectory[i].is_cache_disabled = 0;
		pageDirectory[i].address_low       = (((long) (entries + 1024 * i)) >> 12) % 16;  
		pageDirectory[i].address_high       = ((long) (entries + 1024 * i)) >> 16;  
	}
}



VramMap nicer_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries, BootloaderInfo* bootloaderInfo)
{
	setup_page_directory(pageDirectory, entries);
	uint pos;
	VramMap vram_map;
	for (uint i = (uint)&kernel_text_start; i < (uint)&kernel_memory_end; i += 4096){
		k_mmap(entries, (void*) i, (void*) i, os_occupied);
		pos = i;
	}
	k_mmap(entries, bootloaderInfo, bootloaderInfo, os_occupied);
	pos = (pos > (uint) bootloaderInfo) ? pos : (uint) (bootloaderInfo);
	k_mmap(entries, bootloaderInfo -> mmap_addr, bootloaderInfo -> mmap_addr, os_occupied);
	pos = (pos > (uint) bootloaderInfo -> mmap_addr) ? pos : (uint) (bootloaderInfo -> mmap_addr);
	pos = (pos / 4096 + 1) * 4096;
	pos += 4096;
	vram_map.start = pos;
	BootloaderMapEntry* entry;
	entry = bootloaderInfo -> mmap_addr;
	int i = 0;
	while (i < bootloaderInfo->mmap_length){
		if (entry -> type == 1)
		{
			uint current_addr = (uint) entry -> base_address;
			if (current_addr % 4096)
			{
				current_addr = 4096 * (current_addr / 4096 + 1);
			}
			while (current_addr + 4096 <= entry -> base_address + entry -> length)
			{
				k_mmap(entries, (void*) pos, (void*)current_addr, in_use);
				pos += 4096;
				current_addr += 4096;
			}
		}
		i += entry -> this_size + 4;
		entry = (BootloaderMapEntry*)(((char*)entry) + entry -> this_size + 4);
	}
	vram_map.size = pos - vram_map.start;
	load_page_directory(pageDirectory);
	return vram_map;
}