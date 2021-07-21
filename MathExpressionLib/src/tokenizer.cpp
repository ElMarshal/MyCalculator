#include "exlib/tokenizer.h"

#include <math.h>
#include <assert.h>

#include "exlib/utils.h"
#include "exlib/fmt_string.h"


Tokenizer::Tokenizer(const char* str)
{
	m_str = str;
	m_str_len = strlen(m_str);
	m_begin_it = 0;
	m_it = 0;
	m_line_number = 0;
	m_column_number = 0;
}

const Array<Token>& Tokenizer::tokenize()
{
	while (next_token());

	return m_tokens;
}

const Array<std::string>& Tokenizer::errors() const
{
	return m_errors;
}

bool Tokenizer::next_token()
{
	// skip spaces
	while (peek() == ' ')
	{
		advance();
	}

	// end
	if (peek() == '\0')
	{
		return false;
	}


	// initialize token
	Token token;
	token.line_number = m_line_number;
	token.column_number = m_column_number;
	token.type = TT_UNKNOWN;

	// poarse token
	if (is_digit(peek()))
	{
		// number without sign
		token.type = TT_NUMBER;
		parse_number(&token);
	}
	else if (is_signed_number(peek()) && (m_tokens.size() > 0 && m_tokens.last().type == TT_OPERATOR || m_tokens.size() == 0))
	{
		// signed number = perator + [sign] + number
		// + or - number
		token.type = TT_NUMBER;
		parse_number(&token);
	}
	else if (is_operator(peek()))
	{
		token.type = TT_OPERATOR;
		token.specific.op.v = advance();
	}
	else if (peek() == '(' || peek() == ')')
	{
		token.type = TT_PARENTHESES;
		token.specific.op.v = advance();
	}
	else
	{
		// handle error
		m_errors.push(fmt_string("unexpected character %d:%d: \'%c\'", m_line_number, m_column_number, peek()));
		return false;
	}

	// finish token and add it to the token list
	token.str = get_token_string();
	m_tokens.push(token);
	token_finished();
	return true;
}

char Tokenizer::peek()
{
	return m_str[m_it];
}

char Tokenizer::peek_after_next()
{
	if (m_it + 1 < m_str_len)
	{
		return m_str[m_it+1];
	}
	return '\0';
}

char Tokenizer::advance()
{
	char next_char = peek();

	if (next_char)
	{
		m_it += 1;

		m_column_number += 1;
		if (next_char == '\n')
		{
			m_column_number = 0;
			m_line_number += 1;
		}

		return next_char;
	}
	else
	{
		return '\0';
	}
}

void Tokenizer::token_finished()
{
	m_begin_it = m_it;
}

std::string Tokenizer::get_token_string()
{
	return std::string(&m_str[m_begin_it], m_it - m_begin_it);
}

int Tokenizer::parse_digits()
{
	assert(is_digit(peek()));

	int num = 0;
	while (is_digit(peek()))
	{
		num = num * 10 + char_to_digit(advance());
	}

	return num;
}

Real Tokenizer::parse_signed_number()
{
	int sign = 0;
	if (peek() == '+' || peek() == '-')
	{
		sign = advance() == '-';
	}

	Real number = 0.0;
	int point = 0;

	while (is_digit(peek()))
	{
		number = number * 10.0 + char_to_digit(advance());
		if (point > 0)
		{
			point += 1;
		}
		if (peek() == '.')
		{
			if (point == 0)
			{
				advance();
				point = 1;
			}
			else
			{
				break;
			}
		}

	}

	point = (point > 0) ? (point-1) : 0;

	return (sign ? -1 : 1) * number * pow(10, -point);
}

void Tokenizer::parse_number(Token* token)
{
	Real number = parse_signed_number();
	Real exponent = 0.0;

	// handle exponent
	if (char_to_lowercase(peek()) == 'e' && is_signed_number(peek_after_next()))
	{
		advance();
		exponent = parse_signed_number();
	}

	token->specific.num.v = number * pow(10, exponent);
}


