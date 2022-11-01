#include "../../head/c/asmutils.h"
#include "../../head/c/definitions.h"
#include "../../head/c/kstdlib.h"
#include "../../head/c/gdt.h"
void GDT_Init(GlobalDescriptorTable* gdt){
	kmemset(gdt, 0, sizeof(GlobalDescriptorTable));
	gdt->entries[1].base_low_low = 0;//this is ring0 code segment
	gdt->entries[1].base_high_low = 0;
	gdt->entries[1].base_high_high = 0;
	gdt->entries[1].limit_low = 0xFFFF;
	gdt->entries[1].flags_and_high_limit.Limit = 0xF;
	gdt->entries[1].flags_and_high_limit.Granularity = 1;
	gdt->entries[1].flags_and_high_limit.DB = 1;
	gdt->entries[1].flags_and_high_limit.LongMode = 0;
	gdt->entries[1].access_byte.DirectionConforming = 0;
	gdt->entries[1].access_byte.Executable = 1;
	gdt->entries[1].access_byte.Present = 1;
	gdt->entries[1].access_byte.ProtectionLevel = 0;
	gdt->entries[1].access_byte.ReadWrite = 1;
	gdt->entries[1].access_byte.S = 1;

	gdt->entries[2].base_low_low = 0;//this is ring0 data segment
	gdt->entries[2].base_high_low = 0;
	gdt->entries[2].base_high_high = 0;
	gdt->entries[2].limit_low = 0xFFFF;
	gdt->entries[2].flags_and_high_limit.Limit = 0xF;
	gdt->entries[2].flags_and_high_limit.Granularity = 1;
	gdt->entries[2].flags_and_high_limit.DB = 1;
	gdt->entries[2].flags_and_high_limit.LongMode = 0;
	gdt->entries[2].access_byte.DirectionConforming = 0;
	gdt->entries[2].access_byte.Executable = 0;
	gdt->entries[2].access_byte.Present = 1;
	gdt->entries[2].access_byte.ProtectionLevel = 0;
	gdt->entries[2].access_byte.ReadWrite = 1;
	gdt->entries[2].access_byte.S = 1;

	gdt->entries[3].base_low_low = 0;//this is ring0 code segment
	gdt->entries[3].base_high_low = 0;
	gdt->entries[3].base_high_high = 0;
	gdt->entries[3].limit_low = 0xFFFF;
	gdt->entries[3].flags_and_high_limit.Limit = 0xF;
	gdt->entries[3].flags_and_high_limit.Granularity = 1;
	gdt->entries[3].flags_and_high_limit.DB = 1;
	gdt->entries[3].flags_and_high_limit.LongMode = 0;
	gdt->entries[3].access_byte.DirectionConforming = 0;
	gdt->entries[3].access_byte.Executable = 1;
	gdt->entries[3].access_byte.Present = 1;
	gdt->entries[3].access_byte.ProtectionLevel = 3;
	gdt->entries[3].access_byte.ReadWrite = 1;
	gdt->entries[3].access_byte.S = 1;

	gdt->entries[4].base_low_low = 0;//this is ring0 data segment
	gdt->entries[4].base_high_low = 0;
	gdt->entries[4].base_high_high = 0;
	gdt->entries[4].limit_low = 0xFFFF;
	gdt->entries[4].flags_and_high_limit.Limit = 0xF;
	gdt->entries[4].flags_and_high_limit.Granularity = 1;
	gdt->entries[4].flags_and_high_limit.DB = 1;
	gdt->entries[4].flags_and_high_limit.LongMode = 0;
	gdt->entries[4].access_byte.DirectionConforming = 0;
	gdt->entries[4].access_byte.Executable = 0;
	gdt->entries[4].access_byte.Present = 1;
	gdt->entries[4].access_byte.ProtectionLevel = 3;
	gdt->entries[4].access_byte.ReadWrite = 1;
	gdt->entries[4].access_byte.S = 1;
	
	load_gdt(gdt);
}