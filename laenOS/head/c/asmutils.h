#ifndef ASMUTILS_H
#define ASMUTILS_H
#include "definitions.h"
void outb(short port, unsigned char toSend);
unsigned char inb(short port);
void load_gdt(GlobalDescriptorTable* gdt);
#endif