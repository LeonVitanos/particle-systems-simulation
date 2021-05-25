#include "ConstraintSolver.h"

#include <vector>
#include "Force.h"

ConstraintSolver::ConstraintSolver(std::vector<Force *> constraints) {}

void ConstraintSolver::calculate()
{
    // First get all the J, J_dot, C, C_dot
}