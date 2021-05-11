#pragma once

#include <gfx/vec2.h>

class Force
{
public:
    Force();
    virtual ~Force(void);

    virtual void draw();
    virtual void calculate();
    Vec2f force;
    int id;
};