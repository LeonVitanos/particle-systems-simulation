#include "SlidingConstraint.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

SlidingConstraint::SlidingConstraint(Particle *p, double height) :Force({p}), m_p(p), m_height(height)
{
}

void SlidingConstraint::draw(bool draw[])
{
    if (draw[5]) {
        const double h = 0.03;

		glBegin(GL_LINES);
		glColor3f(0.6, 0.0, 1.0);
		glVertex2f(m_p->m_Position[0], m_p->m_Position[1]);
		glColor3f(0.6, 0.0, 1.0);
		glVertex2f(m_p->m_Position[0] + 50 * this->force[0], m_p->m_Position[1] + 50 * this->force[1]);
		glEnd();

		glPushMatrix();
		glTranslatef(m_p->m_Position[0] + 50 * this->force[0], m_p->m_Position[1] + 50 * this->force[1], 0.0);
		glRotatef((atan2(this->force[1], this->force[0]) * 180) / 3.14159265, 0.0, 0.0, 1.0);
		glColor3f(0.6, 0.0, 1.0);
		glBegin(GL_TRIANGLES);
		glVertex2f(-h / 2.0, -h / 2.0);
		glVertex2f(h / 1.0, 0.0);
		glVertex2f(-h / 2.0, h / 2.0);
		glEnd();
		glPopMatrix();
    }
}

float SlidingConstraint::getC(){
	return m_p->m_Position[1] -  m_height;
}
float SlidingConstraint::getCderivative(){
	return m_p->m_Velocity[1];
}

std::vector<Vec2f> SlidingConstraint::getJ(){
	return std::vector<Vec2f>{Vec2f(0, 1)};
}

std::vector<Vec2f> SlidingConstraint::getJderivative(){
	return std::vector<Vec2f>{Vec2f(0, 0)};
}

void SlidingConstraint::calculate(){}

