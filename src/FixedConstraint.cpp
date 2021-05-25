#include "FixedConstraint.h"

FixedConstraint::FixedConstraint(Particle *particle): Force({particle})
{
    p = particle;
}

void FixedConstraint::draw() {}

void FixedConstraint::calculate()
{
    p->m_Force = Vec2f(0.0, 0.0);
}