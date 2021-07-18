#include "CircularWireConstraint.h"
#include "vector"

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

CircularWireConstraint::CircularWireConstraint(Particle *p, const Vec2f &center, const double radius) :Force({p}), m_p(p), m_center(center), m_radius(radius) {}

float CircularWireConstraint::getC(){
	return ((m_p->m_Position - m_center) * (m_p->m_Position - m_center)) - (m_radius * m_radius);
}
float CircularWireConstraint::getCderivative(){
	return 2 * (m_p->m_Velocity * (m_p->m_Position - m_center));
}

std::vector<Vec2f> CircularWireConstraint::getJ(){
	return std::vector<Vec2f>{2 * (m_p->m_Position - m_center)};
}

std::vector<Vec2f> CircularWireConstraint::getJderivative(){
	return std::vector<Vec2f>{2 * m_p->m_Velocity};
}

void CircularWireConstraint::calculate(){}

void CircularWireConstraint::draw(bool draw[])
{
	draw_circle(m_center, m_radius);

	if (draw[3])
	{
		const double h = 0.03;

		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 1.0);
		glVertex2f(m_p->m_Position[0], m_p->m_Position[1]);
		glColor3f(1.0, 0.0, 1.0);
		glVertex2f(m_p->m_Position[0] + 50 * this->force[0], m_p->m_Position[1] + 50 * this->force[1]);
		glEnd();

		glPushMatrix();
		glTranslatef(m_p->m_Position[0] + 50 * this->force[0], m_p->m_Position[1] + 50 * this->force[1], 0.0);
		glRotatef((atan2(this->force[1], this->force[0]) * 180) / 3.14159265, 0.0, 0.0, 1.0);
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_TRIANGLES);
		glVertex2f(-h / 2.0, -h / 2.0);
		glVertex2f(h / 1.0, 0.0);
		glVertex2f(-h / 2.0, h / 2.0);
		glEnd();
		glPopMatrix();
	}
}
