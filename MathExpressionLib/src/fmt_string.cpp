#include "exlib/fmt_string.h"
#include <vector>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

std::string fmt_string(const char* fmt, ...)
{
    std::vector<char> result;

    va_list args;
    va_start(args, fmt);

    va_list args_temp;
    va_copy(args_temp, args);
    int size = vsnprintf(NULL, 0, fmt, args_temp) + 1; // + 1 to account for the null character
    va_end(args_temp);

    result.resize(size+1, '\0');
    vsnprintf(&result[0], size+1, fmt, args);

    va_end(args);

    return std::string(&result[0]);
}


