#pragma once

#include "array.h"
#include "tokenizer.h"
#include "math_symbols.h"


class MathExpression
{
public:
	MathExpression(const Array<Token>& tokens);

	Real solve(const MathSymbols* symbols = NULL);

	const Array<std::string>& errors() const;

private:
	Array<Token> m_tokens;

	Array<std::string> m_errors;
};

