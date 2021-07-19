#pragma once
#include "array.h"


template <typename T>
class Stack
{
public:
	void push(const T value)
	{
		m_arr.push(value);
	}

	T pop()
	{
		return m_arr.pop();
	}

	size_t size() const
	{
		return m_arr.size();
	}

private:
	Array<T> m_arr;
};

