#include "Scene.h"
void Scene::setup(std::vector<Particle *> &particles, std::vector<Force *> &forces, std::vector<Force *> &constraints, int dragParticle, int scene)
{
    switch(scene){
        case 1:{
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
            forces.push_back((Force *)new SpringForce(particles[0], particles[1], dist, 1, 1));

            // constraints.push_back((Force *)new CircularWireConstraint(particles[0], center, dist));
            constraints.push_back((Force *)new FixedConstraint(particles[1]));
            break;
        }
        case 2:{
            const double dist = 0.2;
            const Vec2f center(0.0, 0.0);
            const Vec2f offset(dist, 0.0);
            Vec2f mouse(0.5, 0.5);

            // Create three particles, attach them to each other, then add a
            // circular wire constraint to the first.

            particles.push_back(new Particle(center + offset));
            particles.push_back(new Particle(center + offset + offset + offset));
            particles.push_back(new Particle(center + offset + offset + offset + offset));
            //pVector.push_back(new Particle(mouse));

            dragParticle = particles.size();

            // Set the spring force
            forces.push_back((Force *)new SpringForce(particles[0], particles[1], dist, 1, 1));

            constraints.push_back((Force *)new CircularWireConstraint(particles[0], center, dist));
            constraints.push_back((Force *)new RodConstraint(particles[1], particles[2], dist * 2));
            break;
        }
        case 3:{ // Cloth
            const double dist = 0.05;
            Vec2f center(0.0, 0.0);
            const Vec2f offset(dist, 0.0);
            Vec2f mouse(0.5, 0.5);

            for (int i = 0; i < 10; i++)
            {
                center = Vec2f(0.0, 0.0 + dist * i);
                for (int j = -4; j <= 4; j++)
                {
                    particles.push_back(new Particle(center + j * offset));
                    //forces.push_back((Force *)new Gravity(particles.back()));
                    //As a first attempt first row of particles could be fixed point
                    if(i==0)
                        constraints.push_back((Force *)new FixedConstraint(particles.back()));
                    if (j != -4)
                        forces.push_back((Force *)new SpringForce(particles[i * 9 + j + 3], particles[i * 9 + j + 4], dist, 1, 1));
                    if (i != 0)
                        forces.push_back((Force *)new SpringForce(particles[i * 9 - 9 + j + 4], particles[i * 9 + j + 4], dist, 1, 1));
                }
            }
            break;
        }
        case 4:{ // Just a spring
            const double dist = 0.2;
            const Vec2f center(0.0, 0.0);
            const Vec2f offset(dist, 0.0);
            Vec2f mouse(0.5, 0.5);

            // Create three particles, attach them to each other, then add a
            // circular wire constraint to the first.

            particles.push_back(new Particle(center - offset));
            particles.push_back(new Particle(center + offset));
            // pVector.push_back(new Particle(center + offset + offset + offset + offset));
            //pVector.push_back(new Particle(mouse));

            dragParticle = particles.size();

            // Set the spring force
            forces.push_back((Force *)new SpringForce(particles[0], particles[1], dist, 1, 1));

            // constraints.push_back((Force *)new RodConstraint(pVector[1], pVector[2], dist * 2));  
        }
    }
};
