#include <gfx/vec2.h>
#include "Particle.h"
#include "Force.h"
#include "SpringForce.h"
#include "Gravity.h"
#include "CircularWireConstraint.h"

#include <vector>

class Scene
{
public:
    Scene();
    ~Scene();
    static void setup(std::vector<Particle *> &particles, std::vector<Force *> &forces, std::vector<Force *> &constraints, int dragParticle, int scene);
};