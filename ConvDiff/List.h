#pragma once
#include <stdio.h>
#include <iostream>

template<class T> class List
{
protected:
	int m_number;
	T** m_item;
public:
	List(int n = 0) :m_number(n), m_item(n ? new T * [n] : 0) {}
	List(int n, const T& t) : m_number(n), m_item(n ? new T * [n] : 0)
	{
		for (int i = 0; i < m_number; i++)
			m_item[i] = new T(t);
	}
	List(const List<T>&);
	const List<T>& operator=(const List<T>&);
	~List()
	{
		for (int i = 0; i < m_number; i++)
			delete m_item[i];
		delete[] m_item;
	}
	int Size() const
	{
		return m_number;
	}
	// () allows modification
	T& operator()(int i)
	{
		if (m_item[i]) return *(m_item[i]);
	}
	// [] reading only
	const T& operator[](int i) const
	{
		if (m_item[i]) return *(m_item[i]);
	}

	//void Print(const List<T>&);
};

template<class T>
List<T>::List(const List<T>& l) :m_number(l.m_number),
m_item(l.m_number ? new T * [l.m_number] : 0)
{
	for (int i = 0; i < l.m_number; i++)
		if (l.m_item[i])
			m_item[i] = new T(*l.m_item[i]);
}

template<class T>
const List<T>&
List<T>::operator=(const List<T>& l)
{
	if (this != &l)
	{
		if (m_number > l.m_number)
			delete[](m_item + l.m_number);
		if (m_number < l.m_number)
		{
			delete[] m_item;
			m_item = new T * [l.m_number];
		}
		for (int i = 0; i < l.m_number; i++)
			if (l.m_item[i])
				m_item[i] = new T(*l.m_item[i]);
		m_number = l.m_number;
	}
	return *this;
}

template<class T>
void Print(const List<T>& l)
{
	for (int i = 0; i < l.Size(); i++)
	{
		std::cout << "i=" << i << "\t";
		print(l[i]);
		std::cout << std::endl;
	}
}