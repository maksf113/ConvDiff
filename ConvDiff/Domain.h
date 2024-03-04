#pragma once
#include "SpaceTimeGrid.h"
#include "Difference.h"

class Domain
{
private:
	SpaceTimeGrid<double> m_grid;
	double m_time;
	double m_width;

public:
	Domain(double time, double width, double accuracy):
		m_time(time), m_width(width), m_grid(int(time/ accuracy) + 1, int(width / accuracy) + 1) {}

	void SolveConvDiff()
	{
		March(m_grid, m_width / m_grid.Width(), m_time / m_grid.TimeSteps());
	}
	double DomainWidth() const { return m_width; }
	int TimeSteps() const { return this->m_grid.Size(); }
	int Width() const { return this->m_grid.Width(); }
	// return single grid point
	double operator() (int i, int j) const
	{
		return double(m_grid(i, j, "read"));
	}

}; 