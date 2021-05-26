#include "FixedConstraint.h"

FixedConstraint::FixedConstraint(Particle *particle)
{
    p = particle;
}

void FixedConstraint::draw(bool draw[]) {}

void FixedConstraint::calculate()
{
    p->m_Force = Vec2f(0.0, 0.0);
}