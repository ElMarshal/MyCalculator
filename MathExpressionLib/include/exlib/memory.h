#pragma once

#include <new>


// types

struct MemoryManagerAllocationsCheck
{
	size_t allocations;
	size_t unfreed_allocations;
	size_t frees;
};


// internal functions
void* mem_malloc_internal(size_t size, const char* file, int line, const char* function);
void* mem_calloc_internal(size_t num, size_t size, const char* file, int line, const char* function);
void* mem_realloc_internal(void* block, size_t size, const char* file, int line, const char* function);
void mem_free_internal(void* block, const char* file, int line, const char* function);
void mem_copy_internal(void* dest, void* src, size_t size, const char* file, int line, const char* function);


// public functions
#define mem_malloc(size) mem_malloc_internal(size, __FILE__, __LINE__, __FUNCTION__)
#define mem_calloc(num, size) mem_calloc_internal(num, size, __FILE__, __LINE__, __FUNCTION__)
#define mem_realloc(block, size) mem_realloc_internal(block, size, __FILE__, __LINE__, __FUNCTION__)
#define mem_free(block) mem_free_internal(block, __FILE__, __LINE__, __FUNCTION__)
#define mem_copy(dest, src, size) mem_copy_internal(dest, src, size, __FILE__, __LINE__, __FUNCTION__)


template<typename T>
T* mem_new()
{
	T* ptr = (T*)mem_malloc(sizeof(T));
	if (ptr)
	{
		new(ptr) T();
	}
	return ptr;
}

template<typename T>
void mem_delete(T* ptr)
{
	if (ptr)
	{
		ptr->~T();
		mem_free(ptr);
	}
}

MemoryManagerAllocationsCheck mem_check_allocs();


