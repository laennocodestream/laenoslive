#ifndef PAGING_H
#define PAGING_H
#include <definitions.h>
#include <asmutils.h>
#define PAGE_PERMISSION_EXEC  1
#define PAGE_PERMISSION_WRITE 2
#define PAGE_PERMISSION_READ  4
enum page_type{
	not_mapped  = 0,
	in_use      = 1,
	os_occupied = 2,
	device      = 3,
	usable      = 4,
	CR3         = 5
};
typedef enum page_type page_type;
typedef struct PageState
{
	page_type type;
	uint pid;
	/* data */
} PageState;

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

typedef struct TransatedAddress
{
	void* virtual;
	void* physical;
}
TransatedAddress;


void load_page_directory(PageDirectoryEntry* pageDirectoryEntry);
void basic_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries);
void nicer_paging_setup(PageDirectoryEntry* pageDirectory, PageTableEntry* entries, BootloaderInfo* bootloaderInfo);
void k_mmap(PageTableEntry* entries, void* virtual, void* physical, PageState destination_state);
void set_page_type(void* physical, page_type destination_state);
void setup_page_directory(PageDirectoryEntry* pageDirectory, PageTableEntry* entries);
int check_if_can_allocate(uint page_count);
void k_allocate(PageTableEntry* entries, uint page_count);
void create_page_directory(PageDirectoryEntry* pageDirectory);
uint find_free_physical_address();
uint find_free_virtual_address();
void create_page_directory(PageDirectoryEntry* pageDirectory);
void add_page_table_to_directory(PageDirectoryEntry* page, uint address);
TransatedAddress create_new_page_directory(uint pid);
void program_mmap(PageDirectoryEntry* directory, void* virtual, void* physical, PageState destination_state, uint permissions, uint pid);
#endif