#include "../../head/c/serial.h"

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