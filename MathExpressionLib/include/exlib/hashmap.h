#pragma once

#include <string>

#include "memory.h"


// HashMap: a separate chaining hash map

#define HASH_MAP_SIZE 16


template <typename K, typename V>
struct KeyValuePair
{
	const K key;
	V value;
};

template <typename K, typename V>
struct HashMapNode
{
	K key;
	V value;
	
	HashMapNode<K, V>* prev;
	HashMapNode<K, V>* next;
};


template <typename K>
size_t hash_key(const K& key);

template <typename K>
size_t hash_key<std::string>(const K& key)
{
	size_t hash = 0;
	for (size_t i = 0; i < key.size(); i++)
	{
		hash += key[i];
	}
	return hash;
}


template <typename K, typename V>
class HashMap
{
public:
	HashMap()
	{
		// zero the m_list_array
		for (size_t i = 0; i < HASH_MAP_SIZE; i++)
		{
			m_list_array[i] = NULL;
		}
	}

	HashMap(const HashMap<K, V>& rhs) :
		HashMap()
	{
		for (size_t i = 0; i < HASH_MAP_SIZE; i++)
		{
			HashMapNode<K, V>* cur = rhs.m_list_array[i];

			while (cur != NULL)
			{
				add(cur->key, cur->value);
				cur = cur->next;
			}
		}
	}

	~HashMap()
	{
		for (size_t i = 0; i < HASH_MAP_SIZE; i++)
		{
			HashMapNode<K, V>* cur = m_list_array[i];
			
			while (cur != NULL)
			{
				HashMapNode<K, V>* next = cur->next;
				mem_delete(cur);
				cur = next;
			}
		}
	}

	HashMap<K, V>& operator=(const HashMap<K, V>& rhs)
	{
		// lazy
		clear();

		for (size_t i = 0; i < HASH_MAP_SIZE; i++)
		{
			HashMapNode<K, V>* cur = rhs.m_list_array[i];

			while (cur != NULL)
			{
				add(cur->key, cur->value);
				cur = cur->next;
			}
		}

		return *this;
	}

	void clear()
	{
		for (size_t i = 0; i < HASH_MAP_SIZE; i++)
		{
			HashMapNode<K, V>* cur = m_list_array[i];

			while (cur != NULL)
			{
				HashMapNode<K, V>* next = cur->next;
				mem_delete(cur);
				cur = next;
			}

			m_list_array[i] = NULL;
		}
	}

	KeyValuePair<K, V>* find(const K& key)
	{
		size_t index = hash_key(key) % HASH_MAP_SIZE;
		HashMapNode<K, V>* root = m_list_array[index];

		HashMapNode<K, V>* found_node = node_find(root, key);
		
		return (KeyValuePair<K, V>*)&found_node->key;
	}

	const KeyValuePair<K, V>* find(const K& key) const
	{
		size_t index = hash_key(key) % HASH_MAP_SIZE;
		const HashMapNode<K, V>* root = m_list_array[index];

		const HashMapNode<K, V>* found_node = node_find(root, key);

		return (const KeyValuePair<K, V>*)&found_node->key;
	}

	bool exists(const K& key) const
	{
		return find(key) != NULL;
	}

	void add(const K& key, const V& value)
	{
		size_t index = hash_key(key) % HASH_MAP_SIZE;
		HashMapNode<K, V>* root = m_list_array[index];

		// null list
		if (!root)
		{
			// add a new node
			HashMapNode<K, V>* new_node = mem_new<HashMapNode<K, V>>();
			new_node->key = key;
			new_node->value = value;
			new_node->prev = NULL;
			new_node->next = NULL;

			m_list_array[index] = new_node;
		}
		else
		{
			// find value in linked list
			HashMapNode<K, V>* found_node = node_find(root, key);

			if (found_node)
			{
				found_node->value = value;
			}
			else
			{
				node_add(root, key, value);
			}
		}
	}

	void remove(const K& key)
	{
		size_t index = hash_key(key) % HASH_MAP_SIZE;
		HashMapNode<K, V>* root = m_list_array[index];

		if (!root)
		{
			return;
		}

		HashMapNode<K, V>* found_node = node_find(root, key);

		if (found_node)
		{
			// the node is the root
			if (found_node == root)
			{
				m_list_array[index] = root->next;
				mem_delete(found_node);
			}
			else
			{
				node_remove(found_node);
			}
		}
	}

private:
	HashMapNode<K, V>* node_find(HashMapNode<K, V>* root, const K& key)
	{
		HashMapNode<K, V>* cur = root;

		while (cur != NULL)
		{
			if (cur->key == key)
			{
				return cur;
			}
			cur = cur->next;
		}

		return NULL;
	}

	const HashMapNode<K, V>* node_find(const HashMapNode<K, V>* root, const K& key) const
	{
		const HashMapNode<K, V>* cur = root;

		while (cur != NULL)
		{
			if (cur->key == key)
			{
				return cur;
			}
			cur = cur->next;
		}

		return NULL;
	}

	void node_add(HashMapNode<K, V>* root, const K& key, const V& value)
	{
		// get last element
		HashMapNode<K, V>* last = root;
		while (last != NULL && last->next != NULL)
		{
			last = last->next;
		}

		// add a new node
		HashMapNode<K, V>* new_node = mem_new<HashMapNode<K, V>>();
		new_node->key = key;
		new_node->value = value;
		new_node->prev = last;
		new_node->next = NULL;

		if (last)
		{
			last->next = new_node;
		}
	}

	void node_remove(HashMapNode<K, V>* node)
	{
		if (node->prev)
		{
			node->prev->next = node->next;
		}

		if (node->next)
		{
			node->next->prev = node->prev;
		}

		mem_delete(node);
	}

private:
	HashMapNode<K, V>* m_list_array[HASH_MAP_SIZE];

};
