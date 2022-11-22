#ifndef KSTDLIB_H
#define KSTDLIB_H
#include <definitions.h>

void kmemset(void* toSet, uint8 toSetTo, uint size);
extern VramMap vram_map;
void* kmalloc(uint size);
void kfree(void* toFree);
void kmalloc_init();

#endif