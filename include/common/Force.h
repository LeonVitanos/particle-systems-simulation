#pragma once

#include <gfx/vec2.h>
#include <gfx/mat2.h>
#include <gfx/vec4.h>
#include <gfx/mat4.h>
#include <vector>
#include <Particle.h>

class Force
{
public:
    Force(std::vector<Particle*> particles);
    virtual ~Force(void);

    virtual void draw(bool draw[]);
    virtual void calculate();

    std::vector<Particle*> particles;
    virtual float getC();
    virtual float getCderivative();
    virtual std::vector<Vec2f> getJ();
    virtual std::vector<Vec2f> getJderivative();
    Vec2f force;
};