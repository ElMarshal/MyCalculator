#include <exlib/math_symbols.h>


MathSymbols default_math_symbols()
{
	MathSymbols symbols;

	// constants
	symbols.constants.add("pi", 3.14159265359);
	symbols.constants.add("e", 2.71828182845);

	// functions
	symbols.functions.add("sqrt", r_sqrt);
	symbols.functions.add("sin", r_sin);
	symbols.functions.add("cos", r_cos);
	symbols.functions.add("tan", r_tan);
	symbols.functions.add("asin", r_asin);
	symbols.functions.add("acos", r_acos);
	symbols.functions.add("atan", r_atan);

	return symbols;
}

