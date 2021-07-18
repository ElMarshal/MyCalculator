#include <stdlib.h>
#include <stdio.h>
#include "array.h"
#include <string>

int main()
{
	Array<int> arr;
	Array<std::string> output;

	// using add and last()
	for (int i = 0; i < 10000; i++)
	{
		arr.add((i^2 + 3*i) % 10);
		output.add(std::to_string(arr.last()));
	}

	// using [] operator
	for (int i = 0; i < 10000; i++)
	{
		output[i] = output[i] + "   " + std::to_string(arr[i]);
	}

	// using first and remove_at
	for (int i = 0; i < 10000; i++)
	{
		output[i] = output[i] + "   " + std::to_string(arr.first());
		arr.remove_at(0);
	}

	output.insert_at("last   |   []   |   first", 0);

	for (int i = 0; i < 10000; i++)
	{
		printf("%s\n", output[i].c_str());
	}

	return 0;
}

