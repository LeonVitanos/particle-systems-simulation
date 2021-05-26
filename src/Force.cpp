#include "Force.h"

Force::Force(std::vector<Particle*> particles): particles(particles)
{
}

Force::~Force() {}

float Force::getC(){return 0;}
float Force::getCderivative(){return 0;}
std::vector<Vec2f> Force::getJ(){return std::vector<Vec2f>{0,0};}
std::vector<Vec2f> Force::getJderivative(){return std::vector<Vec2f>{0,0};}

void Force::draw(bool draw[])
{
    return;
}

void Force::calculate()
{
    return;
}