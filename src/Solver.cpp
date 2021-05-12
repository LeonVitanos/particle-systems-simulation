#include "Particle.h"
#include "Force.h"

#include <vector>

#define DAMP 0.99f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)
void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> forces, float dt, int solver)
{
	int ii, size = pVector.size();

	for (auto &force : forces)
	{
		force->calculate();
	}

	switch (solver)
	{
	case 0: //Euler
		for (ii = 0; ii < size; ii++)
		{

			pVector[ii]->m_Position += dt * pVector[ii]->m_Velocity;
			pVector[ii]->m_Velocity = DAMP * pVector[ii]->m_Velocity + pVector[ii]->m_Force / pVector[ii]->m_Mass * 0.05;
			pVector[ii]->m_Force = Vec2f(0.0, 0.0);
		}
		for (auto &particle : pVector)
		{
			// particle->m_Force = Vec2f(0.0, 0.0);
		}
		break;
	case 1: //MidPoint
		break;
	case 2: //RungeKutta
		break;
	}
}
