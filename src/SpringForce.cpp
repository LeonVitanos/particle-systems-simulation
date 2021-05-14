#include "SpringForce.h"
#include <chrono>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

SpringForce::SpringForce(Particle *p1, Particle *p2, double dist, double ks, double kd) : m_p1(p1), m_p2(p2), m_dist(dist), m_ks(ks), m_kd(kd) {}

void SpringForce::draw()
{
  glBegin(GL_LINES);
  glColor3f(0.0, 0.7, 0.8);
  glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
  glColor3f(1.0, 0.7, 0.2);
  glVertex2f(m_p2->m_Position[0], m_p2->m_Position[1]);
  glEnd();
}

void SpringForce::calculate()
/**
 * f_a: forces on a
 * f_b: forces on b
 * l: a - b 
 * r: rest length
 * k_s: spring constant
 * k_d: dampening constant
 * l_dot: time derivative of l, difference between velocities of a and b
 * f_a = - [ k_s (l_abs - r) + k_d * ( (l_dot * l) / l_abs)] * l/l_abs.
 */
{
  Vec2f l = this->m_p1->m_Position - this->m_p2->m_Position;
  float dist = norm(l);
  Vec2f l_dot = this->m_p1->m_Velocity - this->m_p2->m_Velocity;
  float test = (m_ks * (dist - this->m_dist) + m_kd * (l_dot * l) / dist);
  this->force = test * (l / dist);
  m_p1->m_Force -= this->force;
  m_p2->m_Force += this->force;
}