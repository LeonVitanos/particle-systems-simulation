#include "Particle.h"
#include "Force.h"
#include "Wall.h"
#include "ConstraintSolver.h"

#include <vector>

#define DAMP 0.99f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)
std::vector<double> t;

void Clear_Forces(std::vector<Particle *> pVector)
{
	//Clear forces: zero the force accumulators
	for (auto &particle : pVector)
	{
		particle->m_Force = Vec2f(0.0, 0.0);
	}
}

void Compute_Forces(std::vector<Force *> forces)
{
	for (auto &force : forces)
	{
		force->calculate();
	}
}

void Compute_Collision(Particle *particle, Vec2f position, Vec2f velocity, std::vector<Wall *> walls, float dt)
{
	// Determine if particle crosses wall
	// Current particle coordinates (position vector)
	float p0_x = particle->m_Position[0];
	float p0_y = particle->m_Position[1];

	// Expected particle coordinates
	Vec2f newPos = position + dt * velocity;

	float p1_x = newPos[0];
	float p1_y = newPos[1];

	// Difference (direction vector)
	float s1_x = p1_x - p0_x;
	float s1_y = p1_y - p0_y;

	float mindt = -1;
	int iWall = -1;
	int wSize = walls.size();
	for (int i = 0; i < wSize; i++)
	{
		//Wall endpoint coordinates (p2 as position vector)
		float p2_x = walls[i]->m_p1[0];
		float p3_x = walls[i]->m_p2[0];
		float p2_y = walls[i]->m_p1[1];
		float p3_y = walls[i]->m_p2[1];

		// Difference (direction vector)
		float s2_x = p3_x - p2_x;
		float s2_y = p3_y - p2_y;

		float d = (-s2_x * s1_y + s1_x * s2_y);
		if (!(d == 0))
		{ // if not (near) parallel
			float s = (s1_x * (p0_y - p2_y) - s1_y * (p0_x - p2_x)) / d;
			float t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / d;

			if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
			{ // if intersecting
				if (mindt < 0 || mindt < dt * t)
				{
					mindt = dt * t;
					iWall = i;
				}
			}
		}
	}

	if (mindt < 0)
	{													 // if no intersection
		particle->m_Position = position + dt * velocity; // xdot=v
	}
	else
	{ // if intersection
		// Position where particle hits wall
		newPos = position + (mindt - 0.05f) * velocity;

		// Calculate remaining velocity parallel to the wall
		Vec2f partVeloc = (dt - (mindt - 0.05f)) * velocity;
		Vec2f lineDir = walls[iWall]->m_p1 - walls[iWall]->m_p2;
		float length = sqrt(pow((walls[iWall]->m_p1[0] - walls[iWall]->m_p2[0]), 2) + pow((walls[iWall]->m_p1[1] - walls[iWall]->m_p2[1]), 2));
		lineDir = lineDir / length;
		particle->m_Velocity = ((partVeloc * lineDir) / (lineDir * lineDir)) * lineDir;

		// Final new position
		particle->m_Position = newPos + particle->m_Velocity;
	}
}

void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> forces, std::vector<Force *> constraints, std::vector<Wall *> walls, float dt, int solver)
{
	int ii, size = pVector.size();

	std::vector<Particle *> initial;

	switch (solver)
	{
	case 0: //Euler
		Clear_Forces(pVector);
		Compute_Forces(forces);
		ConstraintSolver(pVector, constraints).calculate();

		for (ii = 0; ii < size; ii++)
		{
			pVector[ii]->m_Velocity += dt * pVector[ii]->m_Force / pVector[ii]->m_Mass; // vdot = f/m

			Compute_Collision(pVector[ii], pVector[ii]->m_Position, pVector[ii]->m_Velocity, walls, dt);
		}

		break;
	case 1: //MidPoint
		Clear_Forces(pVector);
		Compute_Forces(forces);
		ConstraintSolver(pVector, constraints).calculate();

		for (ii = 0; ii < size; ii++)
		{ //Half euler step
			initial.push_back(pVector[ii]);
			pVector[ii]->m_Velocity += (dt / 2) * pVector[ii]->m_Force / pVector[ii]->m_Mass; // vdot = f/m
			//pVector[ii]->m_Position += (dt / 2) * pVector[ii]->m_Velocity;					  // xdot=v

			Compute_Collision(pVector[ii], pVector[ii]->m_Position, pVector[ii]->m_Velocity, walls, dt / 2);
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);
		ConstraintSolver(pVector, constraints).calculate();

		for (ii = 0; ii < size; ii++)
		{
			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + dt * pVector[ii]->m_Force / pVector[ii]->m_Mass; // vdot = f/m
			//pVector[ii]->m_Position = initial[ii]->m_Position + dt * pVector[ii]->m_Velocity;					 // xdot=v

			Compute_Collision(pVector[ii], initial[ii]->m_Position, pVector[ii]->m_Velocity, walls, dt);
		}

		break;
	case 2: //RungeKutta
	{
		std::vector<Vec2f> p1, p2, p3, p4, v1, v2, v3, v4;

		Clear_Forces(pVector);

		for (ii = 0; ii < size; ii++)
		{
			initial.push_back(pVector[ii]);
		}

		Compute_Forces(forces);
		ConstraintSolver(pVector, constraints).calculate();

		for (ii = 0; ii < size; ii++)
		{
			p1.push_back(pVector[ii]->m_Velocity);
			v1.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Velocity += dt / 2 * v1[ii]; // vdot = f/m
			//pVector[ii]->m_Position += dt / 2 * p1[ii]; // xdot=v

			Compute_Collision(pVector[ii], pVector[ii]->m_Position, p1[ii], walls, dt / 2);
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);
		ConstraintSolver(pVector, constraints).calculate();

		for (ii = 0; ii < size; ii++)
		{
			p2.push_back(pVector[ii]->m_Velocity);
			v2.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + dt / 2 * v2[ii]; // vdot = f/m
			//pVector[ii]->m_Position = initial[ii]->m_Position + dt / 2 * p2[ii]; // xdot=v

			Compute_Collision(pVector[ii], initial[ii]->m_Position, p2[ii], walls, dt / 2);
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);
		ConstraintSolver(pVector, constraints).calculate();

		for (ii = 0; ii < size; ii++)
		{
			p3.push_back(pVector[ii]->m_Velocity);
			v3.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + dt * v3[ii]; // vdot = f/m
			//pVector[ii]->m_Position = initial[ii]->m_Position + dt * p3[ii]; // xdot=v

			Compute_Collision(pVector[ii], initial[ii]->m_Position, p3[ii], walls, dt);
		}

		Clear_Forces(pVector);
		Compute_Forces(forces);
		ConstraintSolver(pVector, constraints).calculate();

		for (ii = 0; ii < size; ii++)
		{
			p4.push_back(pVector[ii]->m_Velocity);
			v4.push_back(pVector[ii]->m_Force / pVector[ii]->m_Mass);

			pVector[ii]->m_Velocity = initial[ii]->m_Velocity + 1 / 6 * v1[ii] + 1 / 3 * v2[ii] + 1 / 3 * v3[ii] + 1 / 6 * v4[ii]; // vdot = f/m
			//pVector[ii]->m_Position = initial[ii]->m_Position + 1 / 6 * p1[ii] + 1 / 3 * p2[ii] + 1 / 3 * p3[ii] + 1 / 6 * p4[ii]; // xdot=v

			Compute_Collision(pVector[ii], initial[ii]->m_Position, 1 / 6 * p1[ii] + 1 / 3 * p2[ii] + 1 / 3 * p3[ii] + 1 / 6 * p4[ii], walls, 1);
		}

		break;
	}
	case 3: // Implicit
		break;
	}
}
