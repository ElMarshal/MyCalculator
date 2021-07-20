#pragma once

#include <stdlib.h>
#include <assert.h>
#include <new>
#include <type_traits>

#include "memory.h"


#define ARRAY_INITIAL_CAPACITY 8
#define MAX_SIZE_T(x, y) (((x) > (y)) ? (x) : (y))


template <typename T>
class Array
{
public:
	Array();
    Array(size_t initial_capacity);
	Array(const Array& arr);
	~Array();

	T& at(size_t idx);
	const T& at(size_t idx) const;
	T& operator[](size_t idx);
	const T& operator[](size_t idx) const;
	
	T& first();
	const T& first() const;
	T& last();
	const T& last() const;
	
    void push(const T value);
	void insert_at(const T value, size_t idx);
    T pop();
	void remove(const T value);
	void remove_at(size_t idx);

    int find(const T value) const; // return -1 if not found

	size_t size() const;

private:
	void resize(size_t new_capacity);
	void check_capacity_and_resize(size_t desired_size);

private:
	size_t m_capacity;
	size_t m_size;
	T* m_array;
};


template <typename T>
Array<T>::Array()
{
    m_capacity = ARRAY_INITIAL_CAPACITY;
    m_size = 0;
    m_array = (T*)mem_malloc(m_capacity * sizeof(T));
}

template <typename T>
Array<T>::Array(size_t initial_capacity)
{
    m_capacity = initial_capacity;
    m_size = 0;
    m_array = (T*)mem_malloc(m_capacity * sizeof(T));
}

template <typename T>
Array<T>::Array(const Array& arr) : 
    Array(arr.m_capacity)
{
    for (size_t i = 0; i < arr.m_size; i++)
    {
        push(arr[i]);
    }

    //m_capacity = arr.m_capacity;
    //m_size = arr.m_size;
    //m_array = (T*)mem_malloc(m_capacity * sizeof(T));
    //
    //// call copy constructor for all objects
    //if (std::is_assignable<T, const T&>::value)
    //{
    //    for (size_t i = 0; i < m_size; i++)
    //    {
    //        m_array[i] = arr.m_array[i];
    //    }
    //}
    //else
    //{
    //    mem_copy(m_array, arr.m_array, m_size * sizeof(T));
    //}
}

template <typename T>
Array<T>::~Array()
{
    if (!m_array)
        return;

    // call destructor of all elements
    for (size_t i = 0; i < m_size; i++)
    {
        m_array[i].~T();
    }

    mem_free(m_array);
}

template <typename T>
T& Array<T>::at(size_t idx)
{
    assert(idx < m_size);
    return m_array[idx];
}

template <typename T>
const T& Array<T>::at(size_t idx) const
{
    assert(idx < m_size);
    return m_array[idx];
}

template <typename T>
T& Array<T>::operator[](size_t idx)
{
    return at(idx);
}

template <typename T>
const T& Array<T>::operator[](size_t idx) const
{
    return at(idx);
}

template <typename T>
T& Array<T>::first()
{
    return at(0);
}

template <typename T>
const T& Array<T>::first() const
{
    return at(0);
}

template <typename T>
T& Array<T>::last()
{
    return at(m_size - 1);
}

template <typename T>
const T& Array<T>::last() const
{
    return at(m_size - 1);
}

template <typename T>
void Array<T>::push(const T value)
{
    check_capacity_and_resize(m_size);

    new(&m_array[m_size]) T(value);

    m_size += 1;
}

template <typename T>
void Array<T>::insert_at(const T value, size_t idx)
{
    check_capacity_and_resize(m_size);

    for (size_t i = m_size; i > idx; i--)
    {
        mem_copy(&m_array[i], &m_array[i-1], sizeof(T));
    }

    m_size += 1;
    new(&m_array[idx]) T(value);
}

template <typename T>
T Array<T>::pop()
{
    T result = at(m_size-1);
    remove_at(m_size-1);
    return result;
}

template <typename T>
void Array<T>::remove(const T value)
{
    int found_idx = find(value);
    if (found_idx != -1)
    {
        remove_at(found_idx);
    }
}

template <typename T>
void Array<T>::remove_at(size_t idx)
{
    for (size_t i = idx; i < m_size-1; i++)
    {
        mem_copy(&m_array[i], &m_array[i+1], sizeof(T));
    }

    m_array[m_size-1].~T();

    m_size -= 1;
}

template <typename T>
int Array<T>::find(const T value) const
{
    for (size_t i = 0; i < m_size; i++)
    {
        if (m_array[i] == value)
        {
            return i;
        }
    }
    return -1;
}

template <typename T>
size_t Array<T>::size() const
{
    return m_size;
}

template <typename T>
void Array<T>::resize(size_t new_capacity)
{
    // call destructor of deleted elements
    if (new_capacity < m_size)
    {
        for (size_t i = new_capacity; i < m_size; i++)
        {
            m_array[i].~T();
        }
    }

    void* new_array = mem_realloc(m_array, new_capacity * sizeof(T));
    assert(new_array);

    m_array = (T*)new_array;
    m_capacity = new_capacity;
    if (m_size > m_capacity)
    {
        m_size = m_capacity;
    }
}

template <typename T>
void Array<T>::check_capacity_and_resize(size_t desired_size)
{
    if (desired_size >= m_capacity)
    {
        resize(MAX_SIZE_T(m_capacity * 2, desired_size));
    }
}
