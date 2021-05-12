#include "Gravity.h"

Gravity::Gravity(Particle *particle)
{
    p = particle;
}

Gravity::~Gravity() {}

void Gravity::draw()
{
    // Not implemented
}

void Gravity::calculate()
{
    // TODO: gravity is also really slow
    this->force = Vec2f(0, -(p->m_Mass * 9.81f * 0.01f));
    p->m_Force += this->force;
}
