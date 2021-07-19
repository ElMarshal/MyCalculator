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

	return 0;
}

