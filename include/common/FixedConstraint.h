#pragma once

#include "Particle.h"
#include "Force.h"

class FixedConstraint : Force
{
public:
    FixedConstraint(Particle *p);

    void draw(bool draw[]) override;
    void calculate() override;
    // float getC() override;
    // float getCderivative() override;
    // std::vector<Vec2f> getJ() override;
    // std::vector<Vec2f> getJderivative() override;

private:
    Particle *const m_p;
};