#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include <exlib/tokenizer.h>
#include <exlib/math_expression.h>
#include <exlib/math_symbols.h>

#include "io.h"


static Real solve_math_expression_str(const char* str, const MathSymbols* symbols = NULL)
{
	Tokenizer tokenizer = Tokenizer(str);
	Array<Token> tokens = tokenizer.tokenize();
	Array<std::string> errors = tokenizer.errors();

	if (errors.size() > 0)
	{
		printf("- Parsing error: %s\n", errors[0].c_str());
		return 0.0;
	}

	MathExpression math_exp = MathExpression(tokens);
	Real result = math_exp.solve(symbols);
	errors = math_exp.errors();
	if (errors.size() > 0)
	{
		printf("- Expression error: %s\n", errors[0].c_str());
		return 0.0;
	}

	return result;
}

static int solve_input_expression(bool* loop_again)
{
	printf("> ");

	char* line = readline(stdin);
	if (!line)
	{
		printf("Failed to getline from stdin\n");
		return -1;
	}

	// handle exit
	if (strcmp(line, "exit") == 0)
	{
		mem_free(line);

		*loop_again = false;
		return 0;
	}

	// handle empty line
	if (strlen(line) == 0)
	{
		mem_free(line);
		return 0;
	}
	
	// last result static variables
	static Real last_result = 0.0;
	static MathSymbols symbols = default_math_symbols();

	// update ans
	if (symbols.constants.find("ans"))
	{
		symbols.constants.find("ans")->value = last_result;
	}
	else
	{
		symbols.constants.add("ans", last_result);
	}

	// solve
	Real solution = solve_math_expression_str(line, &symbols);
	printf("\t = %f\n", solution);

	// update last result
	last_result = solution;

	mem_free(line);

	return 0;
}

int main()
{
	// main loop
	int exit_status = 0;
	bool loop = true;

	printf("Input arithmetic expression: \n");

	while (loop)
	{
		int exit_status = solve_input_expression(&loop);
		if (exit_status != 0)
		{
			break;
		}
	}

	MemoryManagerAllocationsCheck memory_check = mem_check_allocs();
	printf("Memory Check:\n - total allocations: %d\n - unfreed allocations: %d\n - total frees: %d\n",
		memory_check.allocations, memory_check.unfreed_allocations, memory_check.frees);

	return exit_status;
}

