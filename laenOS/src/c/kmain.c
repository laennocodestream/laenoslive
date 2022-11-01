#include "../../head/c/kmain.h"
#include "../../head/c/serial.h"
#include "../../head/c/definitions.h"
#include "../../head/c/kstdlib.h"
#include "../../head/c/gdt.h"
#include "../../head/c/paging.h"
BootloaderInfo* bootloaderInfo;
GlobalDescriptorTable gdt;
VramMap vram_map;
extern PageDirectoryEntry page_directory[1024];
Serial com1;
extern PageTableEntry page_entries[1024*1024];
void kmain(BootloaderInfo* info){
	bootloaderInfo = info;
	GDT_Init(&gdt);
	Serial_initialize(&com1, COM1, 9600);
	//basic_paging_setup(page_directory, page_entries);
	vram_map = nicer_paging_setup(page_directory, page_entries, bootloaderInfo);
	kmalloc_init();
	Serial_PrintGDT(&com1, &gdt);
	Serial_writeMemMap(&com1, info);
	Serial_writeStr(&com1, "\n\0");
	Serial_writeHex32(&com1, vram_map.start);
	Serial_writeStr(&com1, "|\0");
	Serial_writeHex32(&com1, vram_map.size);
	Serial_writeStr(&com1, "\n\0");
	char charset[] = "abcdefghijklmnopqrstuvwxyz\n";
	char* ptr1;
	ptr1 = kmalloc(1000);
	Serial_writeHex32(&com1, ptr1);
	Serial_writeStr(&com1, "\n\0");
	char* ptr2;
	ptr2 = kmalloc(1000);
	kfree(ptr1);
	ptr1 = kmalloc(1001);
	Serial_writeHex32(&com1, ptr1);
	Serial_writeStr(&com1, "\n\0");
	while (1)
	{
	}
	
}