#include "memory.h"

#include <stdlib.h>


void* mem_malloc_internal(size_t size, const char* file, int line, const char* function)
{
	return malloc(size);
}

void* mem_calloc_internal(size_t num, size_t size, const char* file, int line, const char* function)
{
	return calloc(num, size);
}

void* mem_realloc_internal(void* block, size_t size, const char* file, int line, const char* function)
{
	return realloc(block, size);
}

void mem_free_internal(void* block, const char* file, int line, const char* function)
{
	free(block);
}

