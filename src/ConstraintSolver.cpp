#include "ConstraintSolver.h"
#include "Eigen/Dense"
#include "Eigen/IterativeLinearSolvers"

#include <vector>
#include "Force.h"

ConstraintSolver::ConstraintSolver(std::vector<Particle *> particles, std::vector<Force *> constraints): particles(particles), constraints(constraints) {}

void ConstraintSolver::calculate()
{
	int nParticles = particles.size();
	int nConstraints = constraints.size();

	//Initializing vectors and matrices with zeros
	Eigen::VectorXf qdot, Q, C, Cdot, Jdotqdot, JWQ, Qhat;
	qdot = Q = Eigen::VectorXf::Zero(2*nParticles);
	C = Cdot = Eigen::VectorXf::Zero(nConstraints);
	Eigen::MatrixXf W, J, Jt, Jdot, JW, JWJt;
	J = Jdot = Eigen::MatrixXf::Zero(nConstraints, 2*nParticles);
	W = Eigen::MatrixXf::Zero(2*nParticles, 2*nParticles);
	Jt = Eigen::MatrixXf::Zero(2*nParticles, nConstraints);
  
	for (int i = 0; i < nParticles; i ++)
	{
		for (int j = 0; j < 2; j++)
		{
			//Inverse of M: diagonal are 1/mass, rest are zero
			W(2*i + j, 2*i + j) = 1/particles[i]->m_Mass;
			//Concatenate the forces on all the particles, to create a global force vector
			Q[2*i + j] = particles[i]->m_Force[j];
			//Concatenate the velocities on all the particles, to create a global velocity vector
			qdot[2*i + j] = particles[i]->m_Velocity[j];
		}
	}

	for (int i = 0; i < nConstraints; i++)
	{
		//Concatenate all the scalar constraint functions to form a single vector function C(q)
		C[i] = constraints[i]->getC();
		//Take derivative of C
		Cdot[i] = constraints[i]->getCderivative();
		//Take Jacobian of C
		std::vector<Vec2f> conJ = constraints[i]->getJ();
		//Take time-derivative of the Jacobian
		std::vector<Vec2f> conJdot = constraints[i]->getJderivative();

		int nPar = constraints[i]->particles.size();
		for (int j = 0; j < nPar; j++)
		{
			// Find particle by iterating all the particles (using a counter does not work for the hair simulation)
			int pos = -1;
			for(int l=0; l<nParticles; l++){
				if(particles[l]==constraints[i]->particles[j]){
					pos=l;
					break;
				}
			}

			for (int k = 0; k < 2; k++)
			{
				J(i,2*pos + k) = conJ[j][k];
				Jt(2*pos + k,i) = conJ[j][k];
				Jdot(i,2*pos + k) = conJdot[j][k];
			}
		}
	}

	JW = J * W;
	JWJt = JW * Jt;
	Jdotqdot = Jdot * qdot;
	JWQ = JW * Q;

	// A conjugate gradient solver for dense self-adjoint problems
	// This class allows to solve for A.x = b linear problems using an iterative conjugate gradient algorithm.
	// For us it is: JWJt*lambda = -Jdotqdot - JWQ - C - Cdot
	Eigen::ConjugateGradient<Eigen::MatrixXf, Eigen::Lower|Eigen::Upper> cg;
	cg.compute(JWJt);
	auto lambda = cg.solve(-Jdotqdot - JWQ - C - Cdot);

	// Once lamba is obtained, it is multiplied by J transpose to obtain Q^,
	// which is added to the applied force before calculating acceleration
	Qhat = Jt * lambda;
	for (int i = 0; i < nParticles; i++)
	{
		if(particles[i]->m_isFixed)
			particles[i]->m_Force = Vec2f(0.0, 0.0);
		else{			
			particles[i]->m_Force[0] += Qhat[i*2];
			particles[i]->m_Force[1] += Qhat[i*2+1];
		}
	}
}