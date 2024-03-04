#pragma once
#include <stdio.h>
#include <iostream>

template<class T>
class DynamicVector 
{
protected:
	int m_dimension;
	T* m_component;
public:
	DynamicVector() : m_dimension(0), m_component(nullptr) {}
	DynamicVector(int, const T& a = 0);
	DynamicVector(const DynamicVector&);
	const DynamicVector& operator=(const DynamicVector&);
	const DynamicVector& operator=(const T&);
	const DynamicVector& operator+=(const DynamicVector&);
	const DynamicVector& operator-=(const DynamicVector&);
	const DynamicVector& operator*=(const T&);
	const DynamicVector& operator/=(const T&);
	T& operator()(int i) { return m_component[i]; } // read / write
	const T& operator[](int i) const { return m_component[i]; } // read only
	int Dim() const{ return m_dimension; }
	~DynamicVector() {
		delete[] m_component;
	}
};

template<class T>
DynamicVector<T>::DynamicVector(int m_dimension, const T& a) :
	m_dimension(m_dimension), m_component(m_dimension ? new T[m_dimension] : 0) 
{
	for (int i = 0; i < m_dimension; i++)
		m_component[i] = a;
}

template<class T>
DynamicVector<T>::DynamicVector(const DynamicVector<T>& v):
	m_dimension(v.m_dimension), m_component(v.m_dimension ? new T[v.m_dimension] : 0)
{
	for (int i = 0; i < v.m_dimension; i++)
		m_component[i] = v.m_component[i];
}

template<class T>
const DynamicVector<T>&
DynamicVector<T>::operator=(const DynamicVector<T>& v)
{
	if (m_dimension > v.m_dimension)
		delete[](m_component + v.m_dimension);
	if (m_dimension < v.m_dimension) {
		delete[] m_component;
		m_component = new T[v.m_dimension];
	}
	for (int i = 0; i < v.m_dimension; i++)
		m_component[i] = v.m_component[i];
	m_dimension = v.m_dimension;
	return *this;
}  

template<class T>
const DynamicVector<T>&
DynamicVector<T>::operator=(const T& a) 
{
	for (int i = 0; i < m_dimension; i++)
		m_component[i] = a;
	return *this;
}

template<class T>
const DynamicVector<T>&
DynamicVector<T>::operator+=(const DynamicVector<T>& v)
{
	for (int i = 0; i < m_dimension; i++)
		m_component[i] += v[i];
	return *this;
}

template<class T>
const DynamicVector<T>&
DynamicVector<T>::operator-=(const DynamicVector<T>& v) 
{
	for (int i = 0; i < m_dimension; i++)
		m_component[i] -= v[i];
	return *this;
}

template<class T>
const DynamicVector<T>&
DynamicVector<T>::operator*=(const T& a) 
{
	for (int i = 0; i < m_dimension; i++)
		m_component[i] *= a;
	return *this;
}

template<class T>
const DynamicVector<T>&
DynamicVector<T>::operator/=(const T& a) 
{
	for (int i = 0; i < m_dimension; i++)
		m_component[i] /= a;
	return *this;
}

template<class T>
const DynamicVector<T>
operator+(const DynamicVector<T>& u, const DynamicVector<T>& v)
{
	return DynamicVector<T>(u) += v;
}

template<class T>
const DynamicVector<T>
operator-(const DynamicVector<T>& u, const DynamicVector<T>& v) 
{
	return DynamicVector<T>(u) -= v;
}

template<class T>
const T
operator*(const DynamicVector<T>& u, const DynamicVector<T>& v) 
{
	T sum = 0.0;
	for (int i; i < u.m_dimension();i++)
		sum += u[i] * v[i];
	return sum;
}

template<class T>
const DynamicVector<T>
operator-(const DynamicVector<T>& v) 
{
	return DynamicVector<T>(v) *= -1;
}

template<class T>
const DynamicVector<T>
operator*(const T& a, const DynamicVector<T>& v) 
{
	return DynamicVector<T>(v) *= a;
}

template<class T>
const DynamicVector<T>
operator*(const DynamicVector<T>& v, const T& a) 
{
	return DynamicVector<T>(v) *= a;
}

template<class T>
const DynamicVector<T>
operator/(const DynamicVector<T>& v, const T& a) 
{
	return DynamicVector<T>(v) /= a;
}

template<class T>
void print(const DynamicVector<T>& v) 
{
	for (int i = 0; i < v.Dim(); i++)
		std::cout << "v[" << i << "] = " << v[i] << std::endl;
}