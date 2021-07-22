#pragma once

#include "memory.h"


template<typename T>
struct LinkedListNode
{
	T value;

	LinkedListNode<T>* prev;
	LinkedListNode<T>* next;
};

template<typename T>
class LinkedList
{
public:
	LinkedList()
	{
		m_root = NULL;
	}

	~LinkedList()
	{
		LinkedListNode<T>* cur = m_root;

		while (cur != NULL)
		{
			LinkedListNode<T>* next = cur->next;
			delete cur;
			cur = next;
		}
	}

	LinkedListNode<T>* find(const T& value)
	{
		LinkedListNode<T>* cur = m_root;

		while (cur != NULL)
		{
			if (cur->value == value)
			{
				return cur;
			}
			cur = cur->next;
		}

		return NULL;
	}

	void add_last(const T& value)
	{
		// get last element
		LinkedListNode<T>* last = m_root;
		while (last != NULL && last->next != NULL)
		{
			last = last->next;
		}

		// add a new node
		LinkedListNode<T>* new_node = (LinkedListNode<T>*)mem_malloc(sizeof(LinkedListNode<T>));
		new_node->value = value;
		new_node->prev = last;
		new_node->next = NULL;

		if (last)
		{
			// add to last element
			last->next = new_node;
		}
		else
		{
			// set as root
			m_root = new_node;
		}
	}

	void node_remove(LinkedListNode<T>* node)
	{
		// delete root node
		if (node == m_root)
		{
			m_root = node->next;

			if (node->next)
			{
				node->next->prev = NULL;
			}
		}
		else
		{
			if (node->prev)
			{
				node->prev->next = node->next;
			}

			if (node->next)
			{
				node->next->prev = node->prev;
			}
		}

		delete node;
	}

private:
	LinkedListNode* m_root;
};
