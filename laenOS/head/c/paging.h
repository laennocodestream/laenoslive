#ifndef PAGING_H
#define PAGING_H
#include "definitions.h"
#include "serial.h"
#include "asmutils.h"
enum page_state{
	not_mapped  = 0,
	in_use      = 1,
	os_occupied = 2,
	device      = 3
};
typedef enum page_state page_state;
struct PageDirectoryEntry
{
	uint8  present           : 1;
	uint8  readwrite         : 1;
	uint8  user_supervision  : 1;
	uint8  PWT               : 1; //to be looked for
	uint8  is_cache_disabled : 1;
	uint8  accessed          : 1;
	uint8  availible         : 1;
	uint8  is_big            : 1;
	uint8  availible_high    : 4;
	uint8  address_low       : 4;
	uint16 address_high;
} __attribute__((packed));
typedef struct PageDirectoryEntry PageDirectoryEntry;


struct PageTableEntry
{
	uint8  present              : 1;
	uint8  readwrite            : 1;
	uint8  user_supervision     : 1;
	uint8  PWT                  : 1; //to be looked for
	uint8  is_cache_disabled    : 1;
	uint8  accessed             : 1;
	uint8  dirty                : 1;
	uint8  page_attribute_table : 1;//can be ignored for now
	uint8  global               : 1;//not to be reload on switch cr3
	uint8  avl                  : 3;
	uint8  address_low          : 4;
	uint16 address_high;
} __attribute__((packed));


typedef struct PageTableEntry PageTableEntry;
void load_page_directory(PageDirectoryEntry* pageDirectoryEntry);
void basic_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries);
VramMap nicer_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries, BootloaderInfo* bootloaderInfo);
void k_mmap(PageTableEntry* entries, void* virtual, void* physical, page_state destination_state);
void setup_page_directory(PageDirectoryEntry* pageDirectory, PageTableEntry* entries);
#endif