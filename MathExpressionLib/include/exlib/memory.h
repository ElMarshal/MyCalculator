#pragma once


// types

struct MemoryManagerAllocationsCheck
{
	size_t allocations;
	size_t unfreed_allocations;
	size_t frees;
};


// public functions
#define mem_malloc(size) mem_malloc_internal(size, __FILE__, __LINE__, __FUNCTION__)
#define mem_calloc(num, size) mem_calloc_internal(num, size, __FILE__, __LINE__, __FUNCTION__)
#define mem_realloc(block, size) mem_realloc_internal(block, size, __FILE__, __LINE__, __FUNCTION__)
#define mem_free(block) mem_free_internal(block, __FILE__, __LINE__, __FUNCTION__)
#define mem_copy(dest, src, size) mem_copy_internal(dest, src, size, __FILE__, __LINE__, __FUNCTION__)

MemoryManagerAllocationsCheck mem_check_allocs();


// internal functions
void* mem_malloc_internal(size_t size, const char* file, int line, const char* function);
void* mem_calloc_internal(size_t num, size_t size, const char* file, int line, const char* function);
void* mem_realloc_internal(void* block, size_t size, const char* file, int line, const char* function);
void mem_free_internal(void* block, const char* file, int line, const char* function);
void mem_copy_internal(void* dest, void* src, size_t size, const char* file, int line, const char* function);

