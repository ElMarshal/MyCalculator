#pragma once
#include <stdio.h>

// readline: reads a line from stream then returns a char* of that line.
// the calee must free the buffer after use.
char* readline(FILE* stream);

