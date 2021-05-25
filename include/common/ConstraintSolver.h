#pragma once

#include "Force.h"
#include <vector>

class ConstraintSolver
{
public:
    ConstraintSolver(std::vector<Force *> particles);

    void calculate();
};