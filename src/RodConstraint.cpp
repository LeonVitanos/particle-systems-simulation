#include "RodConstraint.h"
#include "vector"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

RodConstraint::RodConstraint(Particle *p1, Particle *p2, double dist) :Force({p1, p2}), m_p1(p1), m_p2(p2), m_dist(dist) {
  Vec2f force(0, 0);
  f1 = force;
  f2 = force;
}

float RodConstraint::getC(){
  return (m_p1->m_Position - m_p2->m_Position) * (m_p1->m_Position - m_p2->m_Position) - m_dist * m_dist;
}
float RodConstraint::getCderivative(){
  return (m_p1->m_Position - m_p2->m_Position) * (m_p1->m_Velocity - m_p2->m_Velocity);
}

std::vector<Vec2f> RodConstraint::getJ(){
  return std::vector<Vec2f>{2*(m_p1->m_Position-m_p2->m_Position), 2*(m_p2->m_Position-m_p1->m_Position)};
}

std::vector<Vec2f> RodConstraint::getJderivative(){
	return std::vector<Vec2f>{2*(m_p1->m_Velocity -m_p2->m_Velocity), 2*(m_p2->m_Velocity-m_p1->m_Velocity)};
}

void RodConstraint::calculate(){}

void RodConstraint::draw(bool draw[])
{
  glBegin(GL_LINES);
  glColor3f(0.8, 0.7, 0.6);
  glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
  glColor3f(0.8, 0.7, 0.6);
  glVertex2f(m_p2->m_Position[0], m_p2->m_Position[1]);
  glEnd();

  if (draw[2])
  {
    const double h = 0.03;

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 1.0);
    glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
    glColor3f(0.0, 1.0, 1.0);
    glVertex2f(m_p1->m_Position[0] + 50 * f1[0], m_p1->m_Position[1] + 50 * f1[1]);
    glEnd();

    glPushMatrix();
    glTranslatef(m_p1->m_Position[0] + 50 * f1[0], m_p1->m_Position[1] + 50 * f1[1], 0.0);
    glRotatef((atan2(f1[1], f1[0]) * 180) / 3.14159265, 0.0, 0.0, 1.0);
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-h / 2.0, -h / 2.0);
    glVertex2f(h / 1.0, 0.0);
    glVertex2f(-h / 2.0, h / 2.0);
    glEnd();
    glPopMatrix();

    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(m_p2->m_Position[0], m_p2->m_Position[1]);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(m_p2->m_Position[0] + 50 * f2[0], m_p2->m_Position[1] + 50 * f2[1]);
    glEnd();

    glPushMatrix();
    glTranslatef(m_p2->m_Position[0] + 50 * f2[0], m_p2->m_Position[1] + 50 * f2[1], 0.0);
    glRotatef((atan2(f2[1], f2[0]) * 180) / 3.14159265, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-h / 2.0, -h / 2.0);
    glVertex2f(h / 1.0, 0.0);
    glVertex2f(-h / 2.0, h / 2.0);
    glEnd();
    glPopMatrix();
  }
}
