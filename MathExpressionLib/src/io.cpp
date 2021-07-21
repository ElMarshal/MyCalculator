#include "exlib/io.h"

#include "exlib/array.h"
#include "memory.h"


char* readline(FILE* stream)
{
	Array<char> temp_line;

	// flush stdin
	fseek(stdin, 0, SEEK_END);

	char buffer[128] = { 0 };
	while (fgets(buffer, sizeof(buffer), stdin))
	{
		bool end = false;
		for (size_t i = 0; i < 128 && buffer[i]; i++)
		{
			if (buffer[i] == '\r' || buffer[i] == '\n')
			{
				end = true;
				break;
			}
			temp_line.push(buffer[i]);
		}
		if (end)
		{
			break;
		}
	}

	if (temp_line.size() == 0)
	{
		return NULL;
	}

	size_t line_len = temp_line.size();
	char* line = (char*)mem_malloc((line_len+1) * sizeof(char));
	if (!line)
	{
		return NULL;
	}

	for (size_t i = 0; i < line_len; i++)
	{
		line[i] = temp_line[i];
	}
	line[line_len] = '\0';

	return line;
}

