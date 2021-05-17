#pragma once

#include <gfx/vec2.h>
#include <gfx/mat2.h>
#include <gfx/vec4.h>
#include <gfx/mat4.h>

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