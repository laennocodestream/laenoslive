#include <elf_reader.h>

Program p;



void map_program(ElfHeader32Bits* header)
{
	uint pid = 2;
	TransatedAddress page_directory_address = create_new_page_directory(pid);
	p.pid       = pid;
	p.cr3       = (uint) page_directory_address.physical;
	p.directory = (PageDirectoryEntry*) page_directory_address.virtual;
	ElfProgramHeader32Bits* program_header = (ElfProgramHeader32Bits*) (((char*)header) + header -> program_header_table_position);
	void* program_vram_destination;
	void* physical_destination;
	for (int i = 0; i < header ->program_header_entry_count; i++)
	{
		for (uint j = (uint) program_header[i].segment_pos_virtual_memory; j < (uint) (program_header[i].segment_pos_virtual_memory) + program_header[i].size_in_vram; j+= 4096)
		{
			PageState state;
			state.type = in_use;
			state.pid = pid;
			program_mmap(p.directory, (void*) j, (void*) 0, state, program_header[i].flags, pid);
		}
		program_vram_destination = (void*) program_header[i].segment_pos_virtual_memory;
		uint initial_index = ((uint)program_vram_destination) % 4096;
		char* current_page = find_virtual_address_for_kernel(get_physical_from_virtual_in_directory(p.directory,program_vram_destination));
		for (uint j = 0; j < program_header->size_in_vram; j++)
		{
			if ((initial_index + j) % 4096 == 0)
				current_page = find_virtual_address_for_kernel(get_physical_from_virtual_in_directory(p.directory, ((uint)program_vram_destination) + j));
			current_page[ (initial_index + j) % 4096] = 0;
		}
		char* toWriteFrom = (char*) ((char*) header + program_header[i].segment_offset_in_file);
		initial_index = ((uint)program_vram_destination) % 4096;
		current_page = find_virtual_address_for_kernel(get_physical_from_virtual_in_directory(p.directory,program_vram_destination));
		for (uint j = 0; j < program_header->size_in_file; j++)
		{
			if ((initial_index + j) % 4096 == 0)
				current_page = find_virtual_address_for_kernel(get_physical_from_virtual_in_directory(p.directory, ((uint)program_vram_destination) + j));
			current_page[ (initial_index + j) % 4096] = toWriteFrom[j];
		}
	}
	__asm__("mov %0, %%eax \n"
			"  mov %%eax, %%cr3 \n"
			" call %1" :: "ra" (p.cr3), "rb"(header->program_entry) );
}