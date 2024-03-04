#pragma once
#include "DynamicVector.h"
#include "List.h"

// list of time levels (size = time levels, dim = #grid points)
template<typename T>
class SpaceTimeGrid : public List<DynamicVector<T>>
{
public:
	SpaceTimeGrid(int m = 0, int n = 0, const T& a = 0)
	{
		this->m_number = m;
		this->m_item = m ? new DynamicVector<T>*[m] : 0;
		for (int i = 0; i < m; i++)
			this->m_item[i] = new DynamicVector<T>(n, a);
	}
	int TimeSteps() const { return this->Size(); }
	int Width() const { return this->m_item[0]->Dim(); }

	// return ith time level
	DynamicVector<T>& operator()(int i)
	{
		if (this->m_item[i])
			return *(this->m_item[i]);
	}

	// return single grid point
	T& operator() (int i, int j)
	{
		return (*(this->m_item[i]))(j);
	}

	// return single grid point
	const T& operator() (int i, int j, const char*) const 
	{
		if (this->m_item[i])
		{
			//std::cout << (*(this->m_item[i]))[j];
			//return (*(this->m_item[i]))(j);
			return (*(this->m_item[i]))[j];
		}
		
	}
};




