#include "CircularWireConstraint.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1415926535897932384626433832795

static void draw_circle(const Vec2f &vect, float radius)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 1.0, 0.0);
	for (int i = 0; i < 360; i = i + 18)
	{
		float degInRad = i * PI / 180;
		glVertex2f(vect[0] + cos(degInRad) * radius, vect[1] + sin(degInRad) * radius);
	}
	glEnd();
}

CircularWireConstraint::CircularWireConstraint(Particle *p, const Vec2f &center, const double radius) : m_p(p), m_center(center), m_radius(radius) {}

void CircularWireConstraint::calculate()
{
	/**
	 * p = positions
	 * M = mass
	 * Q = global forces 
	 * C = constraint forces
	 * J = Jacobian of C
	 * k_s = spring constant
	 * k_d = damping constant
	 * JWJ^t \lambda == -J_dot*q_dot - J * W * Q - k_s*C -k_d * C_dot
	 */

	Vec2f position = m_p->m_Position;
	Vec2f velocity = m_p->m_Velocity;
	Vec2f Q = m_p->m_Force;
	Mat2 W = Mat2(Vec2f(1 / m_p->m_Mass, 0), Vec2f(0, 1 / m_p->m_Mass));

	Vec2f J = 2 * (position - m_center);
	Vec2f J_dot = 2 * m_p->m_Velocity;
	double C = ((position - m_center) * (position - m_center)) - (m_radius * m_radius);

	double C_dot = 2 * (velocity * (position - m_center));
	float J_dot_q_dot = J_dot * velocity;
	// Vector-Matrix-Vector multiplication so get ready for ugly code
	float JWQ = J[0] * W[0][0] * Q[0] + J[1] * W[1][1] * Q[1];
	float JWJ = J[0] * W[0][0] * J[0] + J[1] * W[1][1] * J[1];
	float lambda = (0 - J_dot_q_dot - JWQ - C - C_dot) / JWJ;

	m_p->m_Force[0] += lambda * J[0];
	m_p->m_Force[1] += lambda * J[1];
}

void CircularWireConstraint::draw()
{
	draw_circle(m_center, m_radius);
}
