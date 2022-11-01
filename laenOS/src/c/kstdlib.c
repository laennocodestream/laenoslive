#include "../../head/c/kstdlib.h"
void kmemset(void* toSet, uint8 toSetTo, uint size){
	for (uint i = 0; i < size; i++)
	{
		((uint8*)toSet)[i] = toSetTo;
	}
}
typedef struct MallocMeta{
	struct MallocMeta* previous;
	struct MallocMeta* next;
	uint size;
}MallocMeta;

void kmalloc_init(){
	MallocMeta* start = (MallocMeta*) ((void*)vram_map.start);
	MallocMeta* end = (MallocMeta*) ((void*)(vram_map.start + vram_map.size - sizeof(MallocMeta)));
	start -> size = sizeof(MallocMeta);
	start -> previous = NULL;
	start -> next = end;
	end -> previous = start;
	end -> next = NULL;
	end -> size = sizeof(MallocMeta); 
}

void* kmalloc(uint size){
	if (size == 0)
		return NULL;
	MallocMeta* previous = (MallocMeta*) ((void*)vram_map.start);
	MallocMeta* toMalloc;
	while (size + sizeof(MallocMeta) > (uint)(previous -> next) - (uint)(previous) - previous ->size)
	{
		if (previous -> next == NULL)
			return NULL;
		previous = (MallocMeta*) previous -> next;
	}
	toMalloc =(MallocMeta*)( (uint)(previous) +(uint) (previous -> size));
	toMalloc -> size = size + sizeof(MallocMeta);
	toMalloc -> previous = previous;

	toMalloc -> next = previous -> next;
	previous -> next = toMalloc;
	toMalloc -> next -> previous = toMalloc;
	return toMalloc + 1;
}
void kfree(void* toFree)
{
	if (toFree == NULL)
		return;
	MallocMeta* to_free_meta = (MallocMeta*)(((uint)toFree) - sizeof(MallocMeta));
	if (to_free_meta -> next != NULL && to_free_meta -> previous != NULL){
		to_free_meta ->next -> previous = to_free_meta -> previous;
		to_free_meta -> previous -> next = to_free_meta -> next;
	}
}