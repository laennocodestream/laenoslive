#ifndef SERIAL_H
#define SERIAL_H
#include <asmutils.h>
#include <definitions.h>
#include <elf_reader.h>
#define BASE_SERIAL_SPEED 115200 
#define COM1 0x3F8
typedef struct serial{
	short port;
	int initialized;
	int speed;
} Serial;

void Serial_initialize(Serial* toInitialize, short port, int speed);
int Serial_canWrite(Serial* serial);
int Serial_canRead(Serial* serial);
void Serial_writeChar(Serial* serial, char c);
void Serial_writeStr(Serial* serial, char* c);
void Serial_writeHex64(Serial* serial, uint64 toPrint);
void Serial_writeHex32(Serial* serial, uint toPrint);
void Serial_writeHex16(Serial* serial, uint16 toPrint);
void Serial_writebit8(Serial* serial, uint8 toPrint);
void Serial_writeMemMap(Serial* serial, BootloaderInfo* bootloaderInfo);
void Serial_PrintGDT(Serial* serial, GlobalDescriptorTable* gdt);
void Serial_printElfHeader32Bits(Serial* serial, ElfHeader32Bits* header);
unsigned char Serial_readChar(Serial* serial);
#endif