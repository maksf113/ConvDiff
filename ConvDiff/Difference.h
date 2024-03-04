#pragma once
#include "List.h"
#include "DynamicVector.h"

/*
	Convection - Diffusion Equation:
	u = u (t, x), C = C(t, x), F = F(t, x)
	du/dt - epsilon * d^2u/dx^2 + C * du/dt = F

	Initial conditions:
	u(0, x) = u0(x)
	0 < x < L

	Dirichlet boundary condiitons on the right:
	u(t, L) = G(t, L)
	0 < t < T

	Mixed boundary conditions on the left:
	alpha(t, 0) * u(t, 0) + du(t, 0)/dn = G(t, 0)
	0 < t < T
	d/dn - derivative in the outer normal direction (-x in this case (left boundary))
*/

double F(double x, double t) { return 0.0; }
double C(double x, double t) { return 0.1; }
double Alpha(double x, double t) { return 0.0; }
double G(double x, double t) { return 1.5 * x - 1.0; }
double Initial(double x) { return 1.5 - x * x; }
constexpr double Epsilon = 1.0;
constexpr int max_iterations = 100;

// tridiagonal matrix
// list of three dynamic vectors
template<typename T>
class Difference : public List<DynamicVector<T>>
{
public:
	Difference(int n = 0, const T& a = 0, const T& b = 0, const T& c = 0);
	const Difference<T>& operator+=(const Difference<T>&);
	const Difference<T>& operator-=(const Difference<T>&);
	const Difference& operator*=(const T&); // no <T> ?
	T& operator()(int i, int j) // no const - allows modification
	{
		return (*this->m_item[j - i + 1])(i);
	}

	const T& operator()(int i, int j, const char* mode) const
	{
		return (*this->m_item[j - i + 1])[i];
	}

	int Width() // order of the tri-diag matrix
	{
		return this->m_item[0]->Dim();
	}
};

// constructor
template<typename T>
Difference<T>::Difference(int n, const T& a, const T& b, const T& c)
{
	this->m_number = 3;
	this->m_item = new DynamicVector<T>*[3];
	this->m_item[0] = new DynamicVector<T>(n, a);
	this->m_item[1] = new DynamicVector<T>(n, b);
	this->m_item[2] = new DynamicVector<T>(n, c);
}
template<typename T>
const Difference<T>& Difference<T>::operator+=(const Difference<T>& d)
{
	for (int i = 0; i < this->m_number; i++)
		(*this->m_item[i]) += d[i];
	return *this;
}
template<typename T>
const Difference<T>& Difference<T>::operator-=(const Difference<T>& d)
{
	for (int i = 0; i < this->m_number; i++)
		(*this->m_item[i]) -= d[i];
	return *this;
}
template<typename T>
const Difference<T>& Difference<T>::operator*=(const T& a)
{
	for (int i = 0; i < this->m_number; i++)
		(*this->m_item[i]) *= a;
	return *this;
}
// set conv - setts diff operator and right hand side
template<class T>
void ConvDiff(Difference<T>& d, DynamicVector<T>& f, double dx, double dt, double t)
{
	for (int j = 0; j < d.Width(); j++)
	{
		if (t > dt / 2.0)
			f(j) = F(j * dx, t - dt / 2);
		double c = C(j * dx, t);
		// convection term (upwind based on the sign of c)
		if (c > 0.0)
		{
			d(j, j) = c / dx;
			d(j, j - 1) = -c / dx;
			d(j, j + 1) = 0.0;
		}
		else
		{
			d(j, j) = -c / dx;
			d(j, j - 1) = 0.0;
			d(j, j + 1) = c / dx;
		}
	}
	// diffusion term
	d += Epsilon / dx / dx * Difference<T>(d.Width(), -1.0, 2.0, -1.0);

	// boundary conditions
	d(0, 0) += d(0, -1);
	d(0, 0) -= d(0, -1) * dx * Alpha(0, t);
	if (t > dt / 2)
	{
		f(0) -= d(0, -1) * dx * G(0, t - dt / 2.0);
		f(d.Width() - 1) -= d(d.Width() - 1, d.Width()) * G(d.Width() * dx, t - dt / 2.0);
	}
}

template<typename T>
const Difference<T> operator+(const Difference<T>& d1, const Difference<T>& d2)
{
	return Difference<T>(d1) += d2;
}
template<typename T>
const Difference<T> operator-(const Difference<T>& d1, const Difference<T>& d2)
{
	return Difference<T>(d1) -= d2;
}
template<typename T>
const Difference<T> operator*(const T& a, const Difference<T>& d)
{
	return Difference<T>(d) *= a;
}
template<typename T>
const Difference<T> operator*(const Difference<T>& d, const T& a)
{
	return Difference<T>(d) *= a;
}

template<typename T>
inline const T& max(const T& a, const T& b)
{
	return a > b ? a : b;
}
template<typename T>
inline const T& min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
const DynamicVector<T> operator*(const Difference<T>& d, const DynamicVector<T>& v)
{
	DynamicVector<T> dv(v.Dim(), 0);
	for (int i = 0; i < v.Dim(); i++)
		for (int j = max<int>(0, i - 1); j <= min<int>(v.Dim() - 1, i + 1); j++)
			dv(i) += d(i, j, "read") * v[j];
	return dv;
}

// linear solver
template<typename T>
const DynamicVector<T> operator/(const DynamicVector<T>& f, const Difference<T>& d)
{
	DynamicVector<T> x(f);
	for (int iteration = 0; iteration < max_iterations; iteration++)
		for (int i = 0; i < x.Dim(); i++)
		{
			double residual = f[i];
			for (int j = max<int>(0, i - 1); j <= min<int>(x.Dim() - 1, i + 1); j++)
				residual -= d(i, j, "read") * x[j];

			x(i) += residual / d(i, i, "read");
			//print(x);
		}
	return x;
} //solving Dx = f, with max_iterations of Gauss-Seidel

template<typename T>
void March(SpaceTimeGrid<T>& grid, double dx, double dt)
{
	int width = grid.Width();

	Difference<T> I(width, 0.0, 1.0, 0.0); // identity matrix
	for (int j = 0; j < width; j++)
		grid(0, j) = Initial(j * dx);

	DynamicVector<T> f(width);
	Difference<T> previous(width);
	ConvDiff(previous, f, dx, dt, 0);
	print(grid[0]);
	for (int timeStep = 1; timeStep < grid.TimeSteps(); timeStep++)
	{
		Difference<T> current(width);
		ConvDiff(current, f, dx, dt, timeStep * dt);

		grid(timeStep) = ((I - 0.5 * dt * previous) * grid[timeStep - 1] + f * dt)
			/ (I + 0.5 * dt * current); // operator/ as matrix inverse (not explicit)
		previous = current;
	}
}