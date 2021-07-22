#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "exlib/array.h"
#include "exlib/io.h"
#include "exlib/memory.h"
#include "exlib/tokenizer.h"
#include "exlib/math_expression.h"
#include "exlib/utils.h"


static Real solve_math_expression_str(const char* str)
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
	Real result = math_exp.solve();
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
		*loop_again = false;
		return 0;
	}

	Real solution = solve_math_expression_str(line);
	printf("\t = %f\n", solution);

	mem_free(line);

	return 0;
}

static void test_expression(const char* expression, Real expected_result)
{
	Real result = solve_math_expression_str(expression);
	const char* passed_str = (abs(result - expected_result) < 1e-6) ? "PASSED" : "FAILED";
	printf("- [%s] Expression \'%s\' = %f, result = %f\n", passed_str, expression, expected_result, result);
}

static void tests()
{
	// Notes:
	//  tests source: https://lukaszwrobel.pl/blog/math-parser-part-4-tests/
	//  credits to: Lukasz Wrobel

	test_expression("2 + 3", 5);
	test_expression("2 * 3", 6);
	test_expression("89", 89);

	test_expression("   12        -  8   ", 4);
	test_expression("142        -9   ", 133);
	test_expression("72+  15", 87);
	test_expression(" 12*  4", 48);
	test_expression(" 50/10", 5);

	test_expression("2.5", 2.5);
	test_expression("4*2.5 + 8.5+1.5 / 3.0", 19);

	test_expression("2 -4 +6 -1 -1- 0 +8", 10);
	test_expression("1 -1   + 2   - 2   +  4 - 4 +    6", 6);
	test_expression("2 -4 +6 -1 -1- 0 +8", 10);

	test_expression(" 2*3 - 4*5 + 6/3 ", -12);
	test_expression("2*3*4/8 -   5/2*4 +  6 + 0/3   ", -1);

	test_expression("(2) + (17*2-30) * (5)+2 - (8/2)*4", 8);
	test_expression("(((((5)))))", 5);
	test_expression("(( ((2)) + 4))*((5))", 30);
}

int main()
{
	// tests
	tests();
	printf("\n");


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

