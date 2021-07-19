#include "memory.h"

#include <stdlib.h>


MemoryManagerAllocationsCheck g_allocs_check;

MemoryManagerAllocationsCheck mem_check_allocs()
{
	return g_allocs_check;
}

void* mem_malloc_internal(size_t size, const char* file, int line, const char* function)
{
	// add allocated block
	g_allocs_check.allocations += 1;
	g_allocs_check.unfreed_allocations += 1;
	
	return malloc(size);
}

void* mem_calloc_internal(size_t num, size_t size, const char* file, int line, const char* function)
{
	// add allocated block
	g_allocs_check.allocations += 1;
	g_allocs_check.unfreed_allocations += 1;

	return calloc(num, size);
}

void* mem_realloc_internal(void* block, size_t size, const char* file, int line, const char* function)
{
	void* reallocated_ptr = realloc(block, size);

	if (reallocated_ptr)
	{
		// remove old block and added to frees
		g_allocs_check.frees += 1;
		g_allocs_check.unfreed_allocations -= 1;

		// add reallocated block
		g_allocs_check.allocations += 1;
		g_allocs_check.unfreed_allocations += 1;
	}

	return reallocated_ptr;
}

void mem_free_internal(void* block, const char* file, int line, const char* function)
{
	free(block);

	// remove old block and added to frees
	g_allocs_check.frees += 1;
	g_allocs_check.unfreed_allocations -= 1;
}

void mem_copy_internal(void* dest, void* src, size_t size, const char* file, int line, const char* function)
{
	for (size_t i = 0; i < size; i++)
	{
		((char*)dest)[i] = ((char*)src)[i];
	}
}

