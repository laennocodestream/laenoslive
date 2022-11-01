#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#define NULL (void*)0
typedef unsigned char uint8;
typedef unsigned int uint;
typedef unsigned short uint16;
typedef unsigned long long uint64;
extern char kernel_text_start;
extern char kernel_text_end;
extern char kernel_memory_end;
struct BootloaderMapEntry
{
	uint this_size;
	uint64 base_address;
	uint64 length;
	uint type;
} __attribute__((packed));
typedef struct BootloaderMapEntry BootloaderMapEntry;


struct BootloaderInfo {
	uint flags;
	uint mem_lower;
	uint mem_upper;
	uint boot_device;
	uint cmdline;
	uint mods_count;
	uint mods_addr;
	uint symbols[4];
    uint mmap_length;
	BootloaderMapEntry* mmap_addr;
	uint drives_length;
	uint drives_addr;
	uint config_table;
    uint boot_loader_name;
    uint advanced_power_management_table_addr;
	uint vbe_control_info;
	uint vbe_mode_info;
	uint16 vbe_mode;
	uint16 vbe_interface_seg;
	uint16 vbe_interface_off;
	uint16 vbe_interface_len;
    uint64 framebuffer_addr;
	uint framebuffer_pitch;
	uint framebuffer_width;
	uint framebuffer_height;
	uint8 framebuffer_bpp;
	uint8 framebuffer_type;
	uint8 color_info[6];
} __attribute__((packed));
typedef struct BootloaderInfo BootloaderInfo;

struct GlobalDescriptorTableAccessByte
{
	uint8 Accessed:1;
	uint8 ReadWrite:1;
	uint8 DirectionConforming:1;
	uint8 Executable:1;
	uint8 S:1;
	uint8 ProtectionLevel:2;
	uint8 Present:1;
	/* data */
} __attribute__((packed));
typedef struct GlobalDescriptorTableAccessByte GlobalDescriptorTableAccessByte;
struct GlobalDescriptorTableFlagsHighLimit{
	uint8 Limit : 4;
	uint8 reserved : 1;
	uint8 LongMode : 1;
	uint8 DB : 1;
	uint8 Granularity: 1;
} __attribute__((packed));
typedef struct GlobalDescriptorTableFlagsHighLimit GlobalDescriptorTableFlagsHighLimit;
struct GlobalDescriptorTableEntry
{
	uint16 limit_low;
	uint16 base_low_low;
	uint8 base_high_low;
	GlobalDescriptorTableAccessByte access_byte;
	GlobalDescriptorTableFlagsHighLimit flags_and_high_limit;
	uint8 base_high_high;
} __attribute__((packed));
typedef struct GlobalDescriptorTableEntry GlobalDescriptorTableEntry;


struct GlobalDescriptorTable
{
	GlobalDescriptorTableEntry entries[5];
	/* data */
}__attribute__((packed));
typedef struct GlobalDescriptorTable GlobalDescriptorTable;

typedef struct VramMap{
	uint start;
	uint size;
} VramMap;//TODO rename
#endif