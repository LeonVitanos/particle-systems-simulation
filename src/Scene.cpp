#include "Scene.h"
void Scene::setup(std::vector<Particle *> &particles, std::vector<Force *> &forces, std::vector<Force *> &constraints, int dragParticle, int scene)
{
    if (scene == 1)
    {
        const double dist = 0.2;
        const Vec2f center(0.0, 0.0);
        const Vec2f offset(dist, 0.0);
        Vec2f mouse(0.5, 0.5);

        // Create three particles, attach them to each other, then add a
        // circular wire constraint to the first.

        particles.push_back(new Particle(center + offset));
        particles.push_back(new Particle(center + offset + offset + offset));

        dragParticle = particles.size();

        forces.push_back((Force *)new Gravity(particles[0]));

        constraints.push_back((Force *)new CircularWireConstraint(particles[0], center, dist));
    }
    if (scene == 2)
    {
        const double dist = 0.2;
        const Vec2f center(0.0, 0.0);
        const Vec2f offset(dist, 0.0);
        Vec2f mouse(0.5, 0.5);

        // Create three particles, attach them to each other, then add a
        // circular wire constraint to the first.

        particles.push_back(new Particle(center + offset));
        particles.push_back(new Particle(center + offset + offset + offset));
        // pVector.push_back(new Particle(center + offset + offset + offset + offset));
        //pVector.push_back(new Particle(mouse));

        dragParticle = particles.size();

        // Set the spring force
        forces.push_back((Force *)new SpringForce(particles[0], particles[1], dist, 1, 1));

        constraints.push_back((Force *)new CircularWireConstraint(particles[0], center, dist));
        // constraints.push_back((Force *)new RodConstraint(pVector[1], pVector[2], dist * 2));
    }
};
