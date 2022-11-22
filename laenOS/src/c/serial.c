#include <serial.h>

void Serial_initialize(Serial* toInitialize, short port, int speed){
	toInitialize -> port = port;
	toInitialize -> speed = speed;
	outb(port + 1, 0x00);
	outb(port + 3, 0x80);//set the DLAB
	outb(port + 0, (BASE_SERIAL_SPEED / speed) % 256);
	outb(port + 1, (BASE_SERIAL_SPEED / speed) / 256);
	outb(port + 3, 0x3);
	outb(port + 2, 0xC7);
	toInitialize -> initialized = 1;
}
int Serial_canWrite(Serial* serial){
	return inb(serial -> port + 5) & 0x20;
}
int Serial_canRead(Serial* serial){
	return inb(serial -> port + 5) & 0x01;
}
void Serial_writeChar(Serial* serial, char c){
	outb(serial -> port, c);
}
void Serial_writeStr(Serial* serial, char* c){
	int i = 0;
	while (c[i])
	{
		while (!Serial_canWrite(serial));
		Serial_writeChar(serial, c[i]);
		i ++;
	}
	
}
unsigned char Serial_readChar(Serial* serial){
}
void Serial_writeHex64(Serial* serial, uint64 toPrint){
	char tab[19];
	
	tab[0] = '0';
	tab[1] = 'x';
	for (int i = 0; i < 16; i++)
	{
		tab[16 - 1 - i + 2] = "0123456789ABCDEF"[toPrint % 16];
		toPrint /= 16;
	}
	tab[18] = 0;
	Serial_writeStr(serial, tab);
}
void Serial_writeHex32(Serial* serial, uint toPrint){
	char tab[11];
	tab[0] = '0';
	tab[1] = 'x';
	for (int i = 0; i < 8; i++)
	{
		tab[8 - 1 - i + 2] = "0123456789ABCDEF"[toPrint % 16];
		toPrint /= 16;
	}
	tab[10] = 0;
	Serial_writeStr(serial, tab);
}

void Serial_writeHex16(Serial* serial, uint16 toPrint){
	char tab[7];
	tab[0] = '0';
	tab[1] = 'x';
	for (int i = 0; i < 4; i++)
	{
		tab[4 - 1 - i + 2] = "0123456789ABCDEF"[toPrint % 16];
		toPrint /= 16;
	}
	tab[6] = 0;
	Serial_writeStr(serial, tab);
}

void Serial_writebit8(Serial* serial, uint8 toPrint){
	char tab[11];
	tab[0] = '0';
	tab[1] = 'b';
	for (int i = 0; i < 8; i++)
	{
		tab[8 - 1 - i + 2] = "01"[toPrint % 2];
		toPrint /= 2;
	}
	tab[10] = 0;
	Serial_writeStr(serial, tab);
}

void Serial_writeMemMap(Serial* serial, BootloaderInfo* bootloaderInfo){
	Serial_writeStr(serial, "map size -> \0");
	Serial_writeHex32(serial, bootloaderInfo->mmap_length);
	Serial_writeStr(serial, "\n\0");
	BootloaderMapEntry* entry;
	entry = bootloaderInfo -> mmap_addr;
	int i = 0;
	while (i < bootloaderInfo->mmap_length){
		i += entry -> this_size + 4;
		Serial_writeHex64(serial, entry ->base_address);
		Serial_writeStr(serial, "|\0");
		Serial_writeHex64(serial, entry ->length);
		Serial_writeStr(serial, "|\0");
		Serial_writeHex32(serial, entry ->type);
		Serial_writeStr(serial, "\n\0");
		entry = (BootloaderMapEntry*)(((char*)entry) + entry -> this_size + 4);
	}
}

void Serial_PrintGDT(Serial* serial, GlobalDescriptorTable* gdt){
	for (int i = 0 ; i < 5; i++){
		for (int j = 0; j < 8; j++){
			Serial_writebit8(serial, ((uint8*)gdt)[i * 8 + j]);
			Serial_writeStr(serial, " \0");
		}
		Serial_writeStr(serial, "\n\0");
	} 
}

void Serial_printElfHeader32Bits(Serial* serial, ElfHeader32Bits* header){
	char magic_number_to_print[4];
	magic_number_to_print[3] = 0;
	for (int i = 0; i < 3; i ++ )
		magic_number_to_print[i] = (header -> magic_number >> ((i + 1) * 8)) % 256;
	Serial_writeStr(serial, magic_number_to_print);
	Serial_writeStr(serial, "\n\0");
	if (header -> bits == ELF_MODE_32_BITS)
		Serial_writeStr(serial, "32bits \n\0");
	Serial_writeStr(serial, "program entry : \0");
	Serial_writeHex32(serial, header -> program_entry);
	Serial_writeStr(serial, "\nprogram header table position : \0");
	Serial_writeHex32(serial, header -> program_header_table_position);
	Serial_writeStr(serial, "\nprogram header entry count : \0");
	Serial_writeHex16(serial, header -> program_header_entry_count);
	Serial_writeStr(serial, "\nprogram header entry size : \0");
	Serial_writeHex16(serial, header -> program_header_entry_size);
	Serial_writeStr(serial, "\nsection header table position : \0");
	Serial_writeHex32(serial, header -> sections_header_table_position);
	Serial_writeStr(serial, "\nsection header entry count : \0");
	Serial_writeHex16(serial, header -> section_header_entry_count);
	Serial_writeStr(serial, "\nsection header entry size : \0");
	Serial_writeHex16(serial, header -> section_header_entry_size);
	Serial_writeStr(serial, "\n header size : \0");
	Serial_writeHex16(serial, header -> header_size);
	if (header -> bits == ELF_MODE_32_BITS)
	{
		Serial_writeStr(serial, "\nSEGMENTS : \n\0");
		ElfProgramHeader32Bits* program_header = (ElfProgramHeader32Bits*) (((char*)header) + header -> program_header_table_position);
		for (uint16 i = 0; i < header -> program_header_entry_count; i++)
		{
			Serial_writeHex16(serial, i);
			if (program_header[i].segment_type == SEGMENT_LOAD)
			{
				Serial_writeStr(serial, ":LOAD \0");
				Serial_writeStr(serial, ", f_offset \0");
				Serial_writeHex16(serial, program_header[i].segment_offset_in_file);
				Serial_writeStr(serial, ", v_offset \0");
				Serial_writeHex32(serial, program_header[i].segment_pos_virtual_memory);
				Serial_writeStr(serial, ", s_file \0");
				Serial_writeHex32(serial, program_header[i].size_in_file);
				Serial_writeStr(serial, ", s_vram \0");
				Serial_writeHex32(serial, program_header[i].size_in_vram);
				Serial_writeStr(serial, ", perm \0");
				if (program_header[i].flags & SEGMENT_EXEC)
					Serial_writeStr(serial, "E\0");
				if (program_header[i].flags & SEGMENT_WRITE)
					Serial_writeStr(serial, "W\0");
				if (program_header[i].flags & SEGMENT_READ)
					Serial_writeStr(serial, "R\0");
			}
			Serial_writeStr(serial, "\n\0");
		}
	}
}