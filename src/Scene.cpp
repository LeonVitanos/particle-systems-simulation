#include "Scene.h"
#define PI 3.1415926535897932384626433832795

void Scene::setup(std::vector<Particle *> &particles, std::vector<Force *> &forces, std::vector<Force *> &constraints, std::vector<Wall *> &walls, int scene)
{
    Vec2f center(0.0, 0.0);

    switch (scene)
    {
    case 1: // Simple Spring
    {        
        const double dist = 0.6;
        const Vec2f offset(0.9, 0.0);

        particles.push_back(new Particle(center - offset));
        particles.push_back(new Particle(center + offset));

        forces.push_back((Force *)new SpringForce(particles[0], particles[1], dist, 1, 1));
        break;
    }
    case 2: // Spring with FixedConstraint (Pendulum)
    {
        const double dist = 0.3;
        const Vec2f offset(dist, 0.0);

        particles.push_back(new Particle(center - 2 * offset));
        particles.push_back(new Particle(center));

        forces.push_back((Force *)new Gravity(particles[0]));
        forces.push_back((Force *)new SpringForce(particles[0], particles[1], dist, 1, 1));

        constraints.push_back((Force *)new FixedConstraint(particles[1]));
        break;
    }
    case 3: // CircularWire
    {
        const double dist = 0.2;
        const Vec2f offset(dist, 0.0);

        // Create three particles, attach them to each other, then add a
        // circular wire constraint to the first.

        particles.push_back(new Particle(center + offset));
        particles.push_back(new Particle(center + offset + offset + offset));
        particles.push_back(new Particle(center + offset + offset + offset + offset));

        forces.push_back((Force *)new Gravity(particles[0]));
        forces.push_back((Force *)new Gravity(particles[1]));
        forces.push_back((Force *)new Gravity(particles[2]));

        // Set the spring force
        forces.push_back((Force *)new SpringForce(particles[0], particles[1], dist, 1, 1));

        constraints.push_back((Force *)new CircularWireConstraint(particles[0], center, dist));
        constraints.push_back((Force *)new RodConstraint(particles[1], particles[2], dist * 2));
        break;
    }
    case 4:
    { // Cloth
        const double dist = 0.05;
        const Vec2f offset(dist, 0.0);

        for (int i = 0; i < 11; i++)
        {
            center = Vec2f(0.0, 0.0 + 5 * dist - dist * i);
            for (int j = -4; j <= 4; j++)
            {
                particles.push_back(new Particle(center + j * offset));
                forces.push_back((Force *)new Gravity(particles.back()));
                if (i == 0)
                {
                    //Apply fixed constraint on the first row particles
                    constraints.push_back((Force *)new SlidingConstraint(particles.back(), 0.25));
                    if (j != -4)
                    {
                    }
                    // Rod constraints here
                }
                if (j != -4)
                    //spring connecting horizontally
                    forces.push_back((Force *)new SpringForce(particles[i * 9 + j + 3], particles[i * 9 + j + 4], dist, 0.7, 0.8));
                if (i != 0)
                {
                    //spring connecting vertically
                    forces.push_back((Force *)new SpringForce(particles[i * 9 - 9 + j + 4], particles[i * 9 + j + 4], dist, 0.7, 0.8));
                    if (j != 4)
                        //spring connecting diagonally to the right
                        forces.push_back((Force *)new SpringForce(particles[i * 9 + j + 4], particles[i * 9 + j + 4 - 8], dist, 0.05, 0.8));
                    if (j != -4)
                        //spring connecting diagonally to the left
                        forces.push_back((Force *)new SpringForce(particles[i * 9 + j + 4], particles[i * 9 + j + 4 - 10], dist, 0.05, 0.8));
                }
            }
        }

        Vec2f p1(-0.5, 0.6);
        Vec2f p2(-0.5, -0.6);
        walls.push_back(new Wall(p1, p2));

        Vec2f p3(0.5, 0.6);
        Vec2f p4(0.5, -0.6);
        walls.push_back(new Wall(p3, p4));
        break;
    }
    case 5:
    {
        // Angular spring
        const double dist = 0.2;
        const Vec2f offset(dist, 0.0);
        const double hairOffset = 0.05;
        const double hairAngle = 0.003;
        const int hairLength = 8;
        //pythagorean theorem to find distance
        const double hairDis = sqrt(2*pow(hairOffset,2));

        std::vector <Vec2f> hairs;
        hairs.push_back(Vec2f(dist*cos(PI/2.15), dist*sin(PI/2.15)));
        hairs.push_back(Vec2f(dist*cos(PI/2.5), dist*sin(PI/2.5)));
        hairs.push_back(Vec2f(dist*cos(PI/3), dist*sin(PI/3)));
        hairs.push_back(Vec2f(dist*cos(PI/4), dist*sin(PI/4)));
        hairs.push_back(Vec2f(dist*cos(PI/6), dist*sin(PI/6)));
        hairs.push_back(Vec2f(dist*cos(PI/12), dist*sin(PI/12)));
        hairs.push_back(Vec2f(dist*cos(PI-PI/2.15), dist*sin(PI-PI/2.15)));
        hairs.push_back(Vec2f(dist*cos(PI-PI/2.5), dist*sin(PI-PI/2.5)));
        hairs.push_back(Vec2f(dist*cos(PI-PI/3), dist*sin(PI-PI/3)));
        hairs.push_back(Vec2f(dist*cos(PI-PI/4), dist*sin(PI-PI/4)));
        hairs.push_back(Vec2f(dist*cos(PI-PI/6), dist*sin(PI-PI/6)));
        hairs.push_back(Vec2f(dist*cos(PI-PI/12), dist*sin(PI-PI/12)));

        // Create particles for right hair
        for(int n=0; n<hairs.size()/2; n++){
            for (int i=0; i<hairLength; i++){
                const double x=hairOffset-(hairs.size()/2-n)*hairAngle;
                const double y=sqrt(std::abs(pow(hairDis, 2)-pow(x, 2)));
                particles.push_back(new Particle(hairs[n] + Vec2f(i*x, -i*y)));
            }
        }

        // Create particles for left hair
        for(int n=hairs.size()/2; n<hairs.size(); n++){
            for (int i=0; i<hairLength; i++){
                const double x=hairOffset-(hairs.size()-n)*hairAngle;
                const double y=sqrt(std::abs(pow(hairDis, 2)-pow(x, 2)));
                particles.push_back(new Particle(hairs[n] + Vec2f(-i*x, -i*y)));
            }
        }

        // Apply gravity
        for (int i = 0; i < particles.size(); i++)
            forces.push_back((Force *)new Gravity(particles[i]));

        for (int n=0; n<hairs.size(); n++){ //For each hair
            // Set up Angular Springs
            for (int i = 0 + n*hairLength; i < (n+1)*hairLength - 2; i++)
                forces.push_back((Force *)new AngularSpringForce(particles[i], particles[i + 1], particles[i + 2], hairDis, 0.005, 0.1));

            // Set up rod constraints to prevent lenghtening of hair
            for (int i = 0 + n*hairLength; i < (n+1)*hairLength - 1; i++)
                constraints.push_back((Force *)new RodConstraint(particles[i], particles[i + 1], hairDis));

            //Add fixed constraint to each hair's "root"
            constraints.push_back((Force *)new FixedConstraint(particles[n*hairLength]));
        }       

        constraints.push_back((Force *)new CircularWireConstraint(particles[0], center, dist));
    }
    }
};

