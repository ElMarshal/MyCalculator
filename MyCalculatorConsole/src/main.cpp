#include <stdlib.h>
#include <stdio.h>

#include "array.h"
#include "io.h"
#include "memory.h"


int main()
{
	char* line = readline(stdin);
	if (!line)
	{
		printf("Failed to getline from stdin\n");
		return -1;
	}
	
	printf("%s\n", line);

	mem_free(line);

	MemoryManagerAllocationsCheck memory_check = mem_check_allocs();
	printf("Memory Check:\n - total allocations: %d\n - unfreed allocations: %d\n - total frees: %d\n",
		memory_check.allocations, memory_check.unfreed_allocations, memory_check.frees);

	return 0;
}

