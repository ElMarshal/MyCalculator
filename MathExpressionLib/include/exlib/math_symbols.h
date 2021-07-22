#pragma once

#include <string>

#include "real.h"
#include "array.h"
#include "hashmap.h"


using MathFunction = Real(*)(Real param);

struct MathSymbols
{
	HashMap<std::string, Real> constants;
	HashMap<std::string, MathFunction> functions;
};


MathSymbols default_math_symbols();

