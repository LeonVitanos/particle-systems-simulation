#include "Particle.h"
#include "Force.h"

#include <vector>

#define DAMP 0.98f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)
void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> forces, float dt, int solver)
{
	int ii, size = pVector.size();

	//The code given seems to be just randomly setting stuff
	/*	
	for(ii=0; ii<size; ii++)
	{
		pVector[ii]->m_Position += dt*pVector[ii]->m_Velocity;
		pVector[ii]->m_Velocity = DAMP*pVector[ii]->m_Velocity + Vec2f(RAND,RAND) * 0.005;
	}*/

	switch (solver)
	{
	case 0: //Euler

		// Calculate forces
		for (auto &force : forces)
		{
			force->calculate();
		}

		for (ii = 0; ii < size; ii++)
		{
			pVector[ii]->m_Position += dt * pVector[ii]->m_Velocity;
			pVector[ii]->m_Velocity = DAMP * pVector[ii]->m_Velocity + pVector[ii]->m_Force / pVector[ii]->m_Mass * 0.005;
		}
		break;
	case 1: //MidPoint
		break;
	case 2: //RungeKutta
		break;
	}
}
