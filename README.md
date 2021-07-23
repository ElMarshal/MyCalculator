# My Calculator
An arithmetic expression parser that support spaces, parentheses, 
scientific notation, constants and functions written in c++.\
the project also implements common data structures like Dynamic Arrays, Linked List and Hash Map.\
It is stand-alone with no dependencies, except for Qt for MyCalculatorGUI.
It has full syntax and arithmetic error support.


## Screenshots:

### Simple Equations
![Simple Equation](screenshots/1.simple.png) ![Constants and Functions](screenshots/2.func.png)

### Error Handling
![Error handling](screenshots/3.parentheses_error.png) ![Unknown Symbol Error](screenshots/4.unknown_symbol_error.png)


## Components:
* MathExpressionLib: The arithmetic expression parsing library.
* MyCalculatorConsole: A console calculator based on MathExpressionLib.
* MyCalculatorGUI: A GUI calculator based on MathExpressionLib.


## Build:
* Visual Studio: To build MathExpressionLib and MyCalculatorConsole.
* Qt: To build MyCalculatorGUI.


## Code Example:
```Cpp
Real solve_math_expression_str(const char* str)
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
```


## Tests
---
- [PASSED] Expression '2 + 3' = 5.000000, result = 5.000000
- [PASSED] Expression '2 * 3' = 6.000000, result = 6.000000
- [PASSED] Expression '89' = 89.000000, result = 89.000000
- [PASSED] Expression '   12        -  8   ' = 4.000000, result = 4.000000
- [PASSED] Expression '142        -9   ' = 133.000000, result = 133.000000
- [PASSED] Expression '72+  15' = 87.000000, result = 87.000000
- [PASSED] Expression ' 12*  4' = 48.000000, result = 48.000000
- [PASSED] Expression ' 50/10' = 5.000000, result = 5.000000
- [PASSED] Expression '2.5' = 2.500000, result = 2.500000
- [PASSED] Expression '4*2.5 + 8.5+1.5 / 3.0' = 19.000000, result = 19.000000
- [PASSED] Expression '2 -4 +6 -1 -1- 0 +8' = 10.000000, result = 10.000000
- [PASSED] Expression '1 -1   + 2   - 2   +  4 - 4 +    6' = 6.000000, result = 6.000000
- [PASSED] Expression '2 -4 +6 -1 -1- 0 +8' = 10.000000, result = 10.000000
- [PASSED] Expression ' 2*3 - 4*5 + 6/3 ' = -12.000000, result = -12.000000
- [PASSED] Expression '2*3*4/8 -   5/2*4 +  6 + 0/3   ' = -1.000000, result = -1.000000
- [PASSED] Expression '(2) + (17*2-30) * (5)+2 - (8/2)*4' = 8.000000, result = 8.000000
- [PASSED] Expression '(((((5)))))' = 5.000000, result = 5.000000
- [PASSED] Expression '(( ((2)) + 4))*((5))' = 30.000000, result = 30.000000
- [PASSED] Expression 'sin(30*pi/180)' = 0.500000, result = 0.500000
- [PASSED] Expression 'cos(60*pi/180)' = 0.500000, result = 0.500000
---


## References:
* Infix to postfix arithmetic algorithm: https://condor.depaul.edu/ichu/csc415/notes/notes9/Infix.htm
* tests source by Lukasz Wrobel: https://lukaszwrobel.pl/blog/math-parser-part-4-tests/


## Done ✔️

