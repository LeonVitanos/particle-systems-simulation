#include "Gravity.h"

Gravity::Gravity(Particle *particle)
{
    p = particle;
}

Gravity::~Gravity() {}

void Gravity::draw(){}

void Gravity::calculate()
{
    this->force = Vec2f(0, -(p->m_Mass * 9.81f * 0.00001f));
    p->m_Force += this->force - p->m_Velocity*0.01;
}
