#pragma once

#include "array.h"
#include "tokenizer.h"


class MathExpression
{
public:
	MathExpression(const Array<Token>& tokens);

	Real solve();

	const Array<std::string>& errors() const;

private:
	Array<Token> m_tokens;

	Array<std::string> m_errors;
};

