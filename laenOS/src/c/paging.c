#include <paging.h>
#include <asmutils.h>

PageState physical_state[1024*1024] = {};

void basic_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries)
{
	PageState page_state;
	page_state.pid = OS_PID;
	page_state.type = in_use;
	setup_page_directory(pageDirectory, entries);
	for (uint i = 0; i < 1024; i++)
	{
		for (uint j = 0; j < 1024; j++)
		{
			void* addr =(void*) ((i * 1024 + j) * 4 * 1024);
			k_mmap(entries, addr,addr, page_state);
		}	
	}
	load_page_directory(pageDirectory);
}

void set_page_type(void* physical, page_type destination_state)
{
	uint physical_index = ((uint) physical) >> 12; 
	physical_state [physical_index].type = destination_state;

}


void k_mmap(PageTableEntry* entries, void* virtual, void* physical, PageState destination_state)
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

uint find_free_physical_address()
{
	for (uint i = 0; i < 1024*1024; i++)
	{
		if (physical_state[i].type == usable)
			return i << 12;
	}
}

uint find_free_virtual_address()
{
	extern PageTableEntry page_entries[1024*1024];
	extern VramMap vram_map;
	uint virtual_kmemory_selected = (vram_map.start + vram_map.size) >> 12;
	for (uint i = virtual_kmemory_selected + 1; i < 1024*1024; i++)
	{
		if (page_entries[i].present != 1)
			return i << 12;
	}
}

TransatedAddress create_new_page_directory(uint pid){
	extern PageTableEntry page_entries[1024*1024];
	uint virtual_page_table_address = find_free_virtual_address();
	uint physical_page_table_address = find_free_physical_address();
	PageState state;
	state.pid = pid;
	state.type =in_use;
	k_mmap(page_entries, (void*) virtual_page_table_address,(void*) physical_page_table_address, state);
	create_page_directory((PageDirectoryEntry*) virtual_page_table_address);
	TransatedAddress to_return;
	to_return.virtual = (void*) virtual_page_table_address;
	to_return.physical = (void*) physical_page_table_address;
	return to_return;
}

void* find_virtual_address_for_kernel(void* physical){
	extern PageTableEntry page_entries[1024*1024];
	uint physical_shrinked = (uint)(physical) - ((uint) (physical) % 4096);
	for (uint i = 0; i < 1024 * 1024; i++)
	{
		if (page_entries[i].present)
		{
			//TODO conversion function
			if ((page_entries[i].address_low << 12) + (page_entries[i].address_high << 16) == physical_shrinked)
			{
				return (void*) (i * 4096 + ((uint) (physical) % 4096));
			}
		}
	}
	return NULL;
}

void program_mmap(PageDirectoryEntry* directory, void* virtual, void* physical, PageState destination_state, uint permissions, uint pid)
{
	extern PageTableEntry page_entries[1024*1024];
	extern PageDirectoryEntry page_directory[1024];
	uint directory_index = ((uint) virtual) >> 22;
	uint virtual_index  = (((uint) virtual ) >> 12) % 1024;
	uint physical_index = ((uint) physical) >> 12; 
	//add a pagetable to the directory if none is where it's needed
	if (directory[directory_index].present != 1){
		uint virtual_page_table_address = find_free_virtual_address();
		uint physical_page_table_address = find_free_physical_address(); // TODO ADD MEMORY CHECKS 
		PageState state;
		state.pid = pid;
		state.type = in_use;
		k_mmap(page_entries, (void*) virtual_page_table_address,(void*) physical_page_table_address, state);
		add_page_table_to_directory(directory + directory_index, physical_page_table_address);
	}

	if ((uint) physical == 0)
	{
		physical_index = find_free_physical_address() >> 12;
		uint kernel_virtual_index = find_free_virtual_address() >> 12;
		PageState state;
		state.pid = pid;
		state.type = in_use;
		k_mmap(page_entries, (void*)(kernel_virtual_index << 12),(void*)(physical_index << 12), state);
	}
	uint physical_entry = virtual_index + ((uint) directory[directory_index].address_high) << 16 + ((uint) directory[directory_index].address_low) << 12;
	PageTableEntry* entry = (PageTableEntry*) find_virtual_address_for_kernel((void*)physical_entry);
	// TODO add checks that makes this fail if physical state is already used
	physical_state [physical_index] = destination_state;
	entry -> user_supervision     = 1;
	if (permissions | PAGE_PERMISSION_WRITE)
		entry -> readwrite        = 1;
	else
		entry -> readwrite        = 0;
	entry -> PWT                  = 0;
	entry -> present              = 1;
	entry -> page_attribute_table = 0;
	entry -> is_cache_disabled    = 0;
	entry -> global               = 0;
	entry -> address_low          = physical_index % 16;
	entry -> address_high         = physical_index>> 4;
}

void create_page_directory(PageDirectoryEntry* pageDirectory)
{
	for (int i = 0; i < 1024; i++)
	{
		pageDirectory[i].present           = 0;
		pageDirectory[i].is_big            = 0;
		pageDirectory[i].PWT               = 0;
		pageDirectory[i].readwrite         = 1;
		pageDirectory[i].user_supervision  = 1;
		pageDirectory[i].is_cache_disabled = 0;
	}
}

void add_page_table_to_directory(PageDirectoryEntry* page, uint address){
	page -> present = 1;
	page -> address_low = (address >> 12) % 16;
	page -> address_high = address >> 16;
}

void setup_page_directory(PageDirectoryEntry* pageDirectory, PageTableEntry* entries)
{
	create_page_directory(pageDirectory);
	for (int i = 0; i < 1024; i++)
	{
		add_page_table_to_directory(pageDirectory + i, (long)(entries + 1024 * i));
	}
}

void* get_physical_from_virtual_in_directory(PageDirectoryEntry* pageDirectory, void* virtual)
{
	uint directory_index = (uint) virtual >> 22;
	uint pagetable_index = ((uint) virtual >> 12) % 1024;
	void* physical_pagetable_address = (pageDirectory[directory_index].address_low << 12) + (pageDirectory[directory_index].address_high << 16);
	PageTableEntry* virtual_pagetable_address = find_virtual_address_for_kernel(physical_pagetable_address);
	void* page_address = (virtual_pagetable_address[pagetable_index].address_low << 12) + (virtual_pagetable_address[pagetable_index].address_high << 16);
	return (void*)(((uint)page_address) + ((uint)virtual) % 4096);

}


extern VramMap vram_map;
extern uint CR3_HOLDER;
void nicer_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries, BootloaderInfo* bootloaderInfo)
{
	setup_page_directory(pageDirectory, entries);
	uint pos;
	PageState state;
	state.pid = OS_PID;
	state.type = os_occupied;
	for (uint i = (uint)&kernel_text_start; i < (uint)&kernel_memory_end; i += 4096){
		k_mmap(entries, (void*) i, (void*) i, state);
		pos = i;
	}
	k_mmap(entries, bootloaderInfo, bootloaderInfo, state);
	pos = (pos > (uint) bootloaderInfo) ? pos : (uint) (bootloaderInfo);
	k_mmap(entries, bootloaderInfo -> mmap_addr, bootloaderInfo -> mmap_addr, state);
	pos = (pos > (uint) bootloaderInfo -> mmap_addr) ? pos : (uint) (bootloaderInfo -> mmap_addr);
	pos = (pos / 4096 + 1) * 4096;
	pos += 4096;
	vram_map.start = pos;
	BootloaderMapEntry* entry;
	entry = bootloaderInfo -> mmap_addr;
	int i = 0;
	int maped_one_page_for_malloc = 0;
	while (i < bootloaderInfo->mmap_length){
		if (entry -> type == 1)
		{
			uint current_addr = (uint) entry -> base_address;
			if (current_addr % 4096)
			{
				current_addr = 4096 * (current_addr / 4096 + 1);
			}
			state.type = in_use;
			while (current_addr + 4096 <= entry -> base_address + entry -> length)
			{
				if (!maped_one_page_for_malloc && physical_state[current_addr >> 12].type == not_mapped)
				{
					k_mmap(entries, (void*) pos, (void*)current_addr, state);
					maped_one_page_for_malloc = 1;
				}else{
					if (physical_state[current_addr >> 12].type == not_mapped)
						set_page_type((void*)current_addr, usable);
				}
				pos += 4096;
				current_addr += 4096;
			}
		}
		i += entry -> this_size + 4;
		entry = (BootloaderMapEntry*)(((char*)entry) + entry -> this_size + 4);
	}
	vram_map.size = pos - vram_map.start;
	//lock BDA and EBDA
	set_page_type(0, unusable);
	for (uint i = 0x00080000; i < 0x0009FFFF; i += 4096)
		set_page_type((void*) i, unusable);
	set_page_type((void*)&CR3_HOLDER, CR3);
	load_page_directory(pageDirectory);
}

int check_if_can_allocate(uint page_count){
	uint current_page_count = 0;
	for (int i = 0; i < 1024 * 1024; i ++){
		if (physical_state[i].type == usable)
		{
			current_page_count ++;
		}
		if (current_page_count >= page_count)
			return 1;
	}
	return 0;
}
void k_allocate(PageTableEntry* entries, uint page_count){
	uint current_page_count = 0;
	uint current_virtual_addr = vram_map.start;
	PageState page_state;
	page_state.pid = OS_PID;
	page_state.type = in_use;
	while (entries[current_virtual_addr >> 12].present){
		current_virtual_addr += PAGE_SIZE;
	}
	for (int i = 0; i < 1024 * 1024; i ++){
		if (physical_state[i].type == usable)
		{
			k_mmap(entries, (void*) current_virtual_addr,(void*) (i << 12), page_state);
			current_page_count ++;
			current_virtual_addr += PAGE_SIZE;
		}
		if (current_page_count >= page_count)
			return ;
	}
	return;
}