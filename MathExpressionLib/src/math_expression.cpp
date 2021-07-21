#include "exlib/math_expression.h"

#include "exlib/memory.h"
#include "exlib/stack.h"
#include "exlib/fmt_string.h"


// Notes:
//  infix to postfix algorithm: https://condor.depaul.edu/ichu/csc415/notes/notes9/Infix.htm


static int operator_priority(const char c);
static bool is_left_parentheses(const Token& token);
static bool is_right_parentheses(const Token& token);


MathExpression::MathExpression(const Array<Token>& tokens)
{
	m_tokens = tokens;
}

const Array<std::string>& MathExpression::errors() const
{
	return m_errors;
}

Real MathExpression::solve()
{
	// convert from infix to postfix notation
	Array<Token> postfix_tokens;
	Stack<Token> temp_stack;

	for (size_t i = 0; i < m_tokens.size(); i++)
	{
		const Token& cur_token = m_tokens[i];

		if (cur_token.type == TT_NUMBER)
		{
			postfix_tokens.push(cur_token);
		}
		else if (cur_token.type == TT_OPERATOR || cur_token.type == TT_PARENTHESES)
		{
			if (is_right_parentheses(cur_token))
			{
				// right arentheses
				while (temp_stack.size() > 0)
				{
					if (is_left_parentheses(temp_stack.top()))
					{
						temp_stack.pop();
						break;
					}
					postfix_tokens.push(temp_stack.pop());
				}
			}
			else if (is_left_parentheses(cur_token))
			{
				// left arentheses
				temp_stack.push(cur_token);
			}
			else if (cur_token.type == TT_OPERATOR)
			{
				// operator
				while (temp_stack.size() > 0)
				{
					if (is_left_parentheses(temp_stack.top()))
					{
						break;
					}
					else if (operator_priority(temp_stack.top().specific.op.v) < operator_priority(cur_token.specific.op.v))
					{
						break;
					}
					postfix_tokens.push(temp_stack.pop());
				}
				// push operator
				temp_stack.push(cur_token);
			}
		}
	}

	// pop the rest of the stack
	while (temp_stack.size() > 0 && !is_left_parentheses(temp_stack.top()))
	{
		postfix_tokens.push(temp_stack.pop());
	}

	// check for remaining left parenthesis
	if (temp_stack.size() != 0)
	{
		// handle error
		m_errors.push(fmt_string("Unmatched Parenthesis %d:%d: \'(\'", temp_stack.top().line_number, temp_stack.top().column_number));
		return 0.0;
	}

	// evaluate posfix expression
	Stack<Real> num_stack;

	for (size_t i = 0; i < postfix_tokens.size(); i++)
	{
		const Token& cur_token = postfix_tokens[i];

		if (cur_token.type == TT_NUMBER)
		{
			num_stack.push(cur_token.specific.num.v);
		}
		else if (cur_token.type == TT_OPERATOR)
		{
			// check stack for at least 2 operands
			if (num_stack.size() < 2)
			{
				// handle error
				m_errors.push(fmt_string("Missing operand for operator %c %d:%d", cur_token.specific.op.v, cur_token.line_number, cur_token.column_number));
				return 0.0;
			}

			Real rhs = num_stack.pop();
			Real lhs = num_stack.pop();
			Real result = 0.0;

			switch (cur_token.specific.op.v)
			{
			case '+':
				result = lhs + rhs;
				break;
			case '-':
				result = lhs - rhs;
				break;
			case '*':
				result = lhs * rhs;
				break;
			case '/':
				result = lhs / rhs;
				break;
			default:
				break;
			}

			num_stack.push(result);
		}
	}

	if (num_stack.size() == 0)
	{
		// handle error
		m_errors.push("Unbalanced arithmatic expression");
		return 0.0;
	}

	// return value
	return num_stack.pop();
}


// static functions

static int operator_priority(const char c)
{
	switch (c)
	{
	case '+':
	case '-':
		return 1;

	case '*':
	case '/':
		return 2;

	default:
		return 0;
	}
}

static bool is_left_parentheses(const Token& token)
{
	return token.type == TT_PARENTHESES && token.specific.parenthesis.v == '(';
}

static bool is_right_parentheses(const Token& token)
{
	return token.type == TT_PARENTHESES && token.specific.parenthesis.v == ')';
}


