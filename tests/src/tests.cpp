#include <stdlib.h>
#include <stdio.h>

#include <exlib/tokenizer.h>
#include <exlib/math_expression.h>
#include <exlib/math_symbols.h>
#include <exlib/fmt_string.h>


#define RESULT_THRESHOLD 1e-6


enum ErrorType
{
	ET_NO_ERROR = 0,
	ET_TOKENIZER,
	ET_SOLVER,
};

struct ExpressionSolution
{
	Real result;

	ErrorType error_type;
	std::string error_str;
};


static ExpressionSolution solve_math_expression_str(const char* str, const MathSymbols* symbols = NULL)
{
	ExpressionSolution solution;
	solution.result = 0.0;
	solution.error_type = ET_NO_ERROR;

	Tokenizer tokenizer = Tokenizer(str);
	Array<Token> tokens = tokenizer.tokenize();
	Array<std::string> errors = tokenizer.errors();

	if (errors.size() > 0)
	{
		solution.error_type = ET_TOKENIZER;
		solution.error_str = errors[0];
		return solution;
	}

	MathExpression math_exp = MathExpression(tokens);
	Real result = math_exp.solve(symbols);
	errors = math_exp.errors();
	if (errors.size() > 0)
	{
		solution.error_type = ET_SOLVER;
		solution.error_str = errors[0];
		return solution;
	}

	solution.result = result;

	return solution;
}


class Tester
{
public:
	Tester()
	{
		m_fail_count = 0;
		m_symbols = default_math_symbols();
	}

	bool passed() const
	{
		return m_fail_count == 0;
	}

	int fail_count() const
	{
		return m_fail_count;
	}

	// return true if passed
	bool test_expression(const char* expression, Real expected_result);
	bool should_error(const char* expression, ErrorType error_type);

private:
	int m_fail_count;
	MathSymbols m_symbols;
};

bool Tester::test_expression(const char* expression, Real expected_result)
{
	ExpressionSolution solution = solve_math_expression_str(expression, &m_symbols);

	Real abs_delta = abs(solution.result - expected_result);
	if (solution.error_type == ET_NO_ERROR && abs_delta < RESULT_THRESHOLD)
	{
		// passed
		printf(" - TEST EXPRESSION: [PASSED] Expression \'%s\' = %f == %f\n", expression, expected_result, solution.result);
	}
	else if (solution.error_type == ET_NO_ERROR)
	{
		// failed: results don't match
		printf(" - TEST EXPRESSION: [FAILED] Expression \'%s\' = %f != %f\n", expression, expected_result, solution.result);
	}
	else
	{
		// failed: error
		printf(" - TEST EXPRESSION: [FAILED] Expression \'%s\' = %f >>> Error: %s\n", expression, expected_result, solution.error_str.c_str());
	}

	// check overall passed flag
	if (solution.error_type != ET_NO_ERROR)
	{
		m_fail_count += 1;
	}

	return solution.error_type == ET_NO_ERROR;
}

bool Tester::should_error(const char* expression, ErrorType error_type)
{
	ExpressionSolution solution = solve_math_expression_str(expression, &m_symbols);

	if (solution.error_type == error_type)
	{
		// Passed: error type is the same as error_type
		printf(" - SHOULD_ERROR: [PASSED] Expression \'%s\' >>> %s\n", expression, solution.error_str.c_str());
	}
	else
	{
		// failed
		const char* fail_str = "NO ERROR";
		if (solution.error_type != ET_NO_ERROR)
		{
			fail_str = solution.error_str.c_str();
		}

		printf(" - SHOULD_ERROR: [FAILED] Expression \'%s\' >>> %s\n", expression, fail_str);
	}

	// check overall passed flag
	if (solution.error_type != error_type)
	{
		m_fail_count += 1;
	}

	return solution.error_type == error_type;
}


int main()
{
	Tester tester;

	// test number parser
	printf("\n\ntest number parser: \n");
	tester.test_expression("500", 500);
	tester.test_expression("12345", 12345);
	tester.test_expression("1e1", 1e1);
	tester.test_expression("5e-9", 5e-9);
	tester.test_expression("123456789.0", 123456789.0);
	tester.test_expression("0.000005", 0.000005);
	tester.test_expression("+9e-10", +9e-10);
	tester.test_expression("1000e3", 1000e3);
	tester.test_expression("-50e-1", -50e-1);

	// test operators one at a time
	printf("\n\ntest operators one at a time: \n");
	tester.test_expression("5+6", 11);
	tester.test_expression("1-50", -49);
	tester.test_expression("9*8", 72);
	tester.test_expression("-5*4", -20);
	tester.test_expression("-5*-4", 20);
	tester.test_expression("6/2", 3);
	tester.test_expression("5/0.5", 10);
	tester.test_expression("0.9/0.1", 9);
	tester.test_expression("10^2", 100);
	tester.test_expression("2^2", 4);
	tester.test_expression("4^2", 16);
	tester.test_expression("2^3", 8);
	tester.test_expression("25^0.5", 5);
	tester.test_expression("16^0.25", 2);

	// test operator precedence
	printf("\n\ntest operator precedence: \n");
	tester.test_expression("2+5*3", 17);
	tester.test_expression("2*5+3", 13);
	tester.test_expression("2+6*3/2", 11);
	tester.test_expression("90-50+3*2^2", 52);
	tester.test_expression("3+2*3+1", 10);
	tester.test_expression("2^5/4", 8);
	tester.test_expression("3^2/2-4", 0.5);

	// test parentheses
	printf("\n\ntest parentheses: \n");
	tester.test_expression("(2)", 2);
	tester.test_expression("(5)", 5);
	tester.test_expression("(123)", 123);
	tester.test_expression("0.5*(6+2)", 4);
	tester.test_expression("20/(1+4)", 4);
	tester.test_expression("2^(1+1+1)", 8);
	tester.test_expression("(1+1+1)^2", 9);

	// test multiple spaces
	printf("\n\ntest multiple spaces: \n");
	tester.test_expression("    50", 50);
	tester.test_expression("   (1   +10   )   ", 11);
	tester.test_expression("2  ^ 1", 2);
	tester.test_expression("2    ^ 3   ", 8);
	tester.test_expression("5   - 3 ", 2);

	// test multiple parentheses
	printf("\n\ntest multiple parentheses: \n");
	tester.test_expression("((((123))))", 123);
	tester.test_expression("(10)*(5)-10", 40);
	tester.test_expression("(((10)*(5)-10))*((1+1))", 80);

	// test symbols
	printf("\n\ntest symbols: \n");
	tester.test_expression("pi", 3.14159265359);
	tester.test_expression("e", 2.71828182845);
	tester.test_expression("sqrt(1)", 1);
	tester.test_expression("sqrt(4)", 2);
	tester.test_expression("sqrt(9)", 3);
	tester.test_expression("sqrt(16)", 4);
	tester.test_expression("sqrt(25)", 5);
	tester.test_expression("sin(30*pi/180)", 0.5);
	tester.test_expression("cos(60*pi/180)", 0.5);
	for (size_t i = 0; i <= 10; i++)
	{
		Real num = 3.14 * (Real)i/10.0;
		tester.test_expression(fmt_string("sin(%f)", num).c_str(), r_sin(num));
		tester.test_expression(fmt_string("cos(%f)", num).c_str(), r_cos(num));
		tester.test_expression(fmt_string("tan(%f)", num).c_str(), r_tan(num));
	}
	for (size_t i = 0; i <= 10; i++)
	{
		Real num = -1.0 + 2.0*(Real)i/10.0;
		tester.test_expression(fmt_string("asin(%f)", num).c_str(), r_asin(num));
		tester.test_expression(fmt_string("acos(%f)", num).c_str(), r_acos(num));
	}


	// test errors
	printf("\n\ntest errors: \n");
	tester.should_error(".", ET_TOKENIZER);
	tester.should_error("5..", ET_TOKENIZER);
	tester.should_error("61-e.", ET_TOKENIZER);
	tester.should_error("(1", ET_SOLVER);
	tester.should_error("1+", ET_SOLVER);
	tester.should_error("1-", ET_SOLVER);
	tester.should_error("1*", ET_SOLVER);
	tester.should_error("1/", ET_SOLVER);
	tester.should_error("*", ET_SOLVER);
	tester.should_error("/1", ET_SOLVER);
	tester.should_error("unknown_symbol1", ET_SOLVER);
	tester.should_error("unknown_symbol2", ET_SOLVER);
	tester.should_error("sdfsdfsd", ET_SOLVER);
	tester.should_error("sdfsdfsd0(1)", ET_SOLVER);


	// check for overall pass flag
	printf("\n\n");
	if (tester.passed())
	{
		printf("> Overall: [PASSED]\n");
	}
	else
	{
		printf("> Overall: [FAILED]: %d failed\n", tester.fail_count());
	}

	return tester.passed() ? 0 : -1;
}

