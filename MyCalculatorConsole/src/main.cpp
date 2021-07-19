#include <stdlib.h>
#include <stdio.h>

#include "array.h"
#include "io.h"


int main()
{
	char* line = readline(stdin);
	if (!line)
	{
		printf("Failed to getline from stdin\n");
		return -1;
	}
	
	printf("%s\n", line);

	free(line);

	return 0;
}

