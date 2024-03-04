#include "Renderer.h"
#include "Domain.h"

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

	BOUNDARY CONDITIONS and coefficients in "Difference.h" file
*/

int main(void) 
{
	constexpr double time = 10.0;
	constexpr double width = 1.0;
	constexpr double accuracy = 0.01;
	Domain D(time, width, accuracy);
	D.SolveConvDiff();

	constexpr unsigned int window_width = 1920;
	constexpr unsigned int window_height = 1080;
	Renderer renderer(window_width, window_height, D);
	renderer.Render();

	return 0;
}



