#ifndef ELF_READER_H
#define ELF_READER_H
#include <definitions.h>
#include <paging.h>
#define ELF_MODE_32_BITS 1 
#define ELF_MODE_64_BITS 2 
#define SEGMENT_LOAD  1
#define SEGMENT_EXEC  1
#define SEGMENT_WRITE 2
#define SEGMENT_READ  4
struct ElfHeader32Bits
{
	uint magic_number;
	char bits;
	char endianness;
	char elf_header_version;
	char os_ABI;
	uint64 padding;
	uint16 elf_type;
	uint16 instruction_type;
	uint elf_version;
	uint program_entry;
	uint program_header_table_position;
	uint sections_header_table_position;
	uint flags;
	uint16 header_size;
	uint16 program_header_entry_size;
	uint16 program_header_entry_count;
	uint16 section_header_entry_size;
	uint16 section_header_entry_count;
	uint16 section_header_names_index;
} __attribute((packed));

struct ElfProgramHeader32Bits
{
	uint segment_type;
	uint segment_offset_in_file;
	uint segment_pos_virtual_memory;
	uint undefined;
	uint size_in_file;
	uint size_in_vram;
	uint flags;
	uint alignment;
}__attribute((packed));


typedef struct Program
{
	PageDirectoryEntry* directory;
	uint pid;
	uint cr3;
} Program;


typedef struct ElfHeader32Bits ElfHeader32Bits;
typedef struct ElfProgramHeader32Bits ElfProgramHeader32Bits;
void map_program(ElfHeader32Bits* header);
#endif