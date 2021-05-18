#include "RodConstraint.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

RodConstraint::RodConstraint(Particle *p1, Particle *p2, double dist) : m_p1(p1), m_p2(p2), m_dist(dist) {}

void RodConstraint::calculate()
{
  // If you thought the rod constraint was ugly
  Vec2f position1 = m_p1->m_Position;
  Vec2f position2 = m_p2->m_Position;
  Vec2f velocity1 = m_p1->m_Velocity;
  Vec2f velocity2 = m_p2->m_Velocity;
  Vec4f position = Vec4f(position1[0], position1[1], position2[0], position2[1]);
  Vec4f Q = Vec4f(m_p1->m_Force[0], m_p1->m_Force[1], m_p2->m_Force[0], m_p2->m_Force[1]);
  Mat4 W = Mat4(Vec4f(1 / m_p1->m_Mass, 0, 0, 0), Vec4f(0, 1 / m_p1->m_Mass, 0, 0),
                Vec4f(0, 0, 1 / m_p2->m_Mass, 0), Vec4f(0, 0, 0, 1 / m_p2->m_Mass));

  Vec4f J = Vec4f(2 * (position1[0] - position2[0]), 2 * (position1[1] - position2[1]),
                  2 * (position2[0] - position1[0]), 2 * (position2[1] - position1[1]));
  Vec4f Jdot = Vec4f(2 * (velocity1[0] - velocity2[0]), 2 * (velocity1[1] - velocity2[1]),
                     2 * (velocity2[0] - velocity1[0]), 2 * (velocity2[1] - velocity1[1]));

  double C = (position1 - position2) * (position1 - position2) - m_dist * m_dist;
  double Cdot = (position1 - position2) * (velocity1 - velocity2);
  float JWJ = J[0] * W[0][0] * J[0] + J[1] * W[1][1] * J[1] + J[2] * W[2][2] * J[2] + J[3] * W[3][3] * J[3];
  float JWQ = J[0] * W[0][0] * Q[0] + J[1] * W[1][1] * Q[1] + J[2] * W[2][2] * Q[2] + J[3] * W[3][3] * Q[3];
  float Jdotqdot = Jdot[0] * velocity1[0] + Jdot[1] * velocity1[1] + Jdot[2] * velocity2[0] + Jdot[3] * velocity2[1];
  float lambda = (0 - Jdotqdot - JWQ - C - Cdot) / JWJ;

  m_p1->m_Force[0] += lambda * J[0];
  m_p1->m_Force[1] += lambda * J[1];
  m_p2->m_Force[0] += lambda * J[2];
  m_p2->m_Force[1] += lambda * J[3];
}

void RodConstraint::draw()
{
  glBegin(GL_LINES);
  glColor3f(0.8, 0.7, 0.6);
  glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
  glColor3f(0.8, 0.7, 0.6);
  glVertex2f(m_p2->m_Position[0], m_p2->m_Position[1]);
  glEnd();
}
