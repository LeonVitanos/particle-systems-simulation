#pragma once

#include "Force.h"
#include "Particle.h"
class SlidingConstraint : Force
{
public:
    SlidingConstraint(Particle *p1, double height);

    void draw(bool draw[]) override;
    void calculate() override;
    float getC() override;
    float getCderivative() override;
    std::vector<Vec2f> getJ() override;
    std::vector<Vec2f> getJderivative() override;

private:
    Particle *const m_p;
    double const m_height;
};