#include "Particle.h"
#include "Force.h"

#include <vector>

#define DAMP 0.99f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)

void Clear_Forces(std::vector<Particle *> pVector){
	//Clear forces: zero the force accumulators
	for (auto &particle : pVector){
		particle->m_Force = Vec2f(0.0, 0.0);
	}
}

void Compute_Forces(std::vector<Force *> forces){
	for (auto &force : forces){
			force->calculate();
	}
}

void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> forces, float dt, int solver)
{
	int ii, size = pVector.size();

	std::vector<Particle *> initial;

	switch (solver)
	{
	case 0: //Euler
		Clear_Forces(pVector);
		Compute_Forces(forces);

		for (ii=0; ii<size; ii++){
			pVector[ii]->m_Position += dt * pVector[ii]->m_Velocity; // xdot=v
			pVector[ii]->m_Velocity += dt * pVector[ii]->m_Force / pVector[ii]->m_Mass; // vdot = f/m
		}
		break;
	case 1: //MidPoint
		Clear_Forces(pVector);
		
		for (ii=0; ii<size; ii++){
			initial.push_back(pVector[ii]);
		}
		Compute_Forces(forces);

		for (ii=0; ii<size; ii++){
			pVector[ii]->m_Position += dt/2 * pVector[ii]->m_Velocity; // xdot=v
			pVector[ii]->m_Velocity += dt/2 * pVector[ii]->m_Force / pVector[ii]->m_Mass; // vdot = f/m
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);

		for (ii=0; ii<size; ii++){
			pVector[ii]->m_Position = initial[ii]->m_Position + dt * pVector[ii]->m_Velocity; // xdot=v
			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + dt * pVector[ii]->m_Force / pVector[ii]->m_Mass; // vdot = f/m
		}

		break;
	case 2: //RungeKutta
		std::vector<Vec2f> p1,p2,p3,p4,v1,v2,v3,v4;

		Clear_Forces(pVector);
		
		for (ii=0; ii<size; ii++){
			initial.push_back(pVector[ii]);
		}

		Compute_Forces(forces);

		for (ii=0; ii<size; ii++){
			p1.push_back(pVector[ii]->m_Velocity);
			v1.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Position += dt/2 * p1[ii]; // xdot=v
			pVector[ii]->m_Velocity += dt/2 * v1[ii]; // vdot = f/m
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);

		for (ii=0; ii<size; ii++){
			p2.push_back(pVector[ii]->m_Velocity);
			v2.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Position = initial[ii]->m_Position + dt/2 * p2[ii]; // xdot=v
			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + dt/2 * v2[ii]; // vdot = f/m
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);

		for (ii=0; ii<size; ii++){
			p3.push_back(pVector[ii]->m_Velocity);
			v3.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Position = initial[ii]->m_Position + dt * p3[ii]; // xdot=v
			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + dt * v3[ii]; // vdot = f/m
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);

		for (ii=0; ii<size; ii++){
			p4.push_back(pVector[ii]->m_Velocity);
			v4.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Position = initial[ii]->m_Position + 1/6*p1[ii] + 1/3*p2[ii] + 1/3*p3[ii] + 1/6*p4[ii]; // xdot=v
			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + 1/6*v1[ii] + 1/3*v2[ii] + 1/3*v3[ii] + 1/6*v4[ii]; // vdot = f/m
		}
		break;
	}
}

