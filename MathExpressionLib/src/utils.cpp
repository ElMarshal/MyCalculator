#include <exlib/utils.h>


bool is_operator(const char c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
		return true;
	}

	return false;
}

bool is_digit(const char c)
{
	return (c >= '0') && (c <= '9');
}

bool is_signed_number(const char c)
{
	return c == '+' || c == '-' || is_digit(c);
}

bool is_alphabit(const char c)
{
	return is_lowercase(c) || is_uppercase(c);
}

int char_to_digit(const char c)
{
	if (is_digit(c))
	{
		return c - '0';
	}
	return -1;
}

bool is_lowercase(const char c)
{
	return (c >= 'a') && (c <= 'z');
}

bool is_uppercase(const char c)
{
	return (c >= 'A') && (c <= 'Z');
}

char char_to_lowercase(const char c)
{
	if (is_uppercase(c))
	{
		return c + ('a' - 'A');
	}
	return c;
}

char char_to_uppercase(const char c)
{
	if (is_lowercase(c))
	{
		return c - ('a' - 'A');
	}
	return c;
}

std::string str_to_lowercase(const std::string& str)
{
	std::string result;

	for (size_t i = 0; i < str.size(); i++)
	{
		result += char_to_lowercase(str[i]);
	}

	return result;
}

std::string str_to_uppercase(const std::string& str)
{
	std::string result;

	for (size_t i = 0; i < str.size(); i++)
	{
		result += char_to_uppercase(str[i]);
	}

	return result;
}

