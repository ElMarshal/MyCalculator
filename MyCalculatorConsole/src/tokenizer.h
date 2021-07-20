#pragma once

#include <string>

#include "array.h"


typedef double Real;

enum TokenType
{
	TT_UNKNOWN = 0,
	TT_OPERATOR,
	TT_NUMBER,
	TT_PARENTHESES,
};

struct Token
{
	size_t line_number;
	size_t column_number;
	std::string str;
	TokenType type;
	union
	{
		struct
		{
			char v;
		} op;

		struct
		{
			Real v;
		} num;

		struct
		{
			char v;
		} parenthesis;
	} specific;
};


class Tokenizer
{
public:
	Tokenizer(const char* str);

	Array<Token> tokenize();

private:
	bool next_token();
	char peek(); // returns the next char without advancing
	char peek_after_next(); // returns the next after next char without advancing
	char advance(); // returns the next char and advances
	void token_finished(); // sets m_begin_it = m_it
	std::string get_token_string(); // gets the string of the current token = m_str(m_begin_it:m_it)

	int parse_digits();
	Real parse_signed_number();
	void parse_number(Token* token);

private:
	const char* m_str;
	size_t m_str_len;
	size_t m_begin_it;
	size_t m_it;
	size_t m_line_number;
	size_t m_column_number;

	Array<Token> m_tokens;
};

