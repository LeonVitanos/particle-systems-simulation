#pragma once

#include "Particle.h"
#include "Force.h"

class RodConstraint : Force
{
public:
  RodConstraint(Particle *p1, Particle *p2, double dist);

  void draw(bool draw[]) override;
  void calculate() override;
  float getC() override;
  float getCderivative() override;
  std::vector<Vec2f> getJ() override;
  std::vector<Vec2f> getJderivative() override;

private:
  Particle *const m_p1;
  Particle *const m_p2;
  double const m_dist;
  Vec2f f1;
  Vec2f f2;
};
