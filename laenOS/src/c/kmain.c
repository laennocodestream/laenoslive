#include <kmain.h>
#include <serial.h>
#include <definitions.h>
#include <kstdlib.h>
#include <gdt.h>
#include <paging.h>
#include <idt.h>
#include <elf_reader.h>
BootloaderInfo* bootloaderInfo;
GlobalDescriptorTable gdt;
IdtPointer idtPointer;
VramMap vram_map;
extern PageDirectoryEntry page_directory[1024];
Serial com1;
extern PageTableEntry page_entries[1024*1024];
extern IdtGateDescriptor idt_entries[256];
extern ElfHeader32Bits program_start;
void kmain(BootloaderInfo* info){
	bootloaderInfo = info;
	GDT_Init(&gdt);
	Serial_initialize(&com1, COM1, 9600);
	//basic_paging_setup(page_directory, page_entries);
	nicer_paging_setup(page_directory, page_entries, bootloaderInfo);
	kmalloc_init();
	make_and_load_idt(idt_entries, &idtPointer);
	Serial_PrintGDT(&com1, &gdt);
	Serial_writeMemMap(&com1, info);
	Serial_writeStr(&com1, "\n\0");
	Serial_writeHex32(&com1, vram_map.start);
	Serial_writeStr(&com1, "|\0");
	Serial_writeHex32(&com1, vram_map.size);
	Serial_writeStr(&com1, "\n\0");
	Serial_printElfHeader32Bits(&com1, &program_start);
	map_program(&program_start);
	//asm("int $0x80");
	while (1)
	{
	}
	
}