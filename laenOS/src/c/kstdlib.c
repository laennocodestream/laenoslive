#include <kstdlib.h>
#include <paging.h>
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
	MallocMeta* end = (MallocMeta*) ((void*)(vram_map.start + PAGE_SIZE - sizeof(MallocMeta)));
	start -> size = sizeof(MallocMeta);
	start -> previous = NULL;
	start -> next = end;
	end -> previous = start;
	end -> next = NULL;
	end -> size = sizeof(MallocMeta); 
}

extern PageTableEntry page_entries[1024*1024];

void* kmalloc(uint size){
	if (size == 0)
		return NULL;
	MallocMeta* previous = (MallocMeta*) ((void*)vram_map.start);
	MallocMeta* toMalloc;
	while (previous-> next == NULL || size + sizeof(MallocMeta) > (uint)(previous -> next) - (uint)(previous) - previous ->size)
	{
		if (previous -> next == NULL)
		{
			previous = previous -> previous;
			int number_of_pages_to_allocate = size + sizeof(MallocMeta) - ((uint)(previous -> next) - (uint)(previous) - previous ->size);
			if (number_of_pages_to_allocate % PAGE_SIZE != 0)	
				number_of_pages_to_allocate += PAGE_SIZE;
			number_of_pages_to_allocate /= PAGE_SIZE;
			if (check_if_can_allocate(number_of_pages_to_allocate))
			{
				k_allocate(page_entries, number_of_pages_to_allocate);
				MallocMeta* end = (MallocMeta*)((char*) previous -> next + number_of_pages_to_allocate * PAGE_SIZE);
				end -> previous = previous;
				end -> next = NULL;
				end -> size = sizeof(MallocMeta); 
				previous -> next = end;
				return kmalloc(size);
			}
			else {
				return NULL;
			}
		}
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