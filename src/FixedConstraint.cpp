#include "FixedConstraint.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

FixedConstraint::FixedConstraint(Particle *p): Force({p}), m_p(p)
{
}

void FixedConstraint::draw(bool draw[]) {
    if (draw[4]) {
        const double h = 0.03;

		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.6);
		glVertex2f(m_p->m_Position[0], m_p->m_Position[1]);
		glColor3f(1.0, 0.0, 0.6);
		glVertex2f(m_p->m_Position[0] + 50 * this->force[0], m_p->m_Position[1] + 50 * this->force[1]);
		glEnd();

		glPushMatrix();
		glTranslatef(m_p->m_Position[0] + 50 * this->force[0], m_p->m_Position[1] + 50 * this->force[1], 0.0);
		glRotatef((atan2(this->force[1], this->force[0]) * 180) / 3.14159265, 0.0, 0.0, 1.0);
		glColor3f(1.0, 0.0, 0.6);
		glBegin(GL_TRIANGLES);
		glVertex2f(-h / 2.0, -h / 2.0);
		glVertex2f(h / 1.0, 0.0);
		glVertex2f(-h / 2.0, h / 2.0);
		glEnd();
		glPopMatrix();
    }
}

void FixedConstraint::calculate(){}

// float FixedConstraint::getC(){
// 	return m_p->m_Position*m_p->m_Position;
// }
// float FixedConstraint::getCderivative(){
// 	return 2*m_p->m_Velocity*m_p->m_Position;
// }

// std::vector<Vec2f> FixedConstraint::getJ(){
// 	return std::vector<Vec2f>{2*m_p->m_Position};
// }

// std::vector<Vec2f> FixedConstraint::getJderivative(){
// 	return std::vector<Vec2f>{2*m_p->m_Velocity};
// }