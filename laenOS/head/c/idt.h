#ifndef IDT_H
#define IDT_H
#include <definitions.h>
#define INTERRUPT_GATE_TYPE 0xE
#define TRAP_GATE_TYPE      0xF
struct IdtGateDescriptor
{
	uint16 offset_low;
	uint16 segment_selector;
	uint8 reserved;
	uint8 gate_type         : 4;
	uint8 zero              : 1;
	uint8 caller_protection : 2;
	uint8 present           : 1;
	uint16 offset_high;
} __attribute__((packed));

typedef struct IdtGateDescriptor IdtGateDescriptor;
struct IdtPointer{
	uint16 entries_size;
	IdtGateDescriptor* entry_pos;
	} __attribute__((packed));
typedef struct IdtPointer IdtPointer;

void __c_interrupt_vector(uint interruptCode,uint edi, uint esi, uint ebp, uint esp, uint ebx, uint edx, uint ecx, uint eax, uint error_code);
void make_and_load_idt(IdtGateDescriptor* entries, IdtPointer* idtPointer);
#endif