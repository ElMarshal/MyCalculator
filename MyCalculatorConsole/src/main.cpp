#include <stdlib.h>
#include <stdio.h>
#include "array.h"
#include <string>

int main()
{
	size_t test_size = 5000;
	Array<int> arr;
	Array<std::string> output;

	// using add and last()
	for (size_t i = 0; i < test_size; i++)
	{
		arr.add((i^2 + 3*i) % 10);
		output.add(std::to_string(arr.last()));
	}

	// using [] operator
	for (size_t i = 0; i < test_size; i++)
	{
		output[i] = output[i] + "   " + std::to_string(arr[i]);
	}

	// using first and remove_at
	for (size_t i = 0; i < test_size; i++)
	{
		output[i] = output[i] + "   " + std::to_string(arr.first());
		arr.remove_at(0);
	}

	output.insert_at("last   |   []   |   first", 0);

	for (size_t i = 0; i < test_size; i++)
	{
		printf("%s\n", output[i].c_str());
	}

	return 0;
}

