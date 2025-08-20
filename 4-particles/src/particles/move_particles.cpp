// src/particles/move_particles.cpp
#include <SFML/Graphics.hpp>

#include <particles.h>

void move_particles(Particles& particles, Camera& camera) {

    sf::Vector2u size = camera.window->getSize();
    const float HALF_WIDTH = 1000.f;
    const float HALF_HEIGHT = 1000.f;
    const float HALF_DEPTH = 1000.f;
    const float dt = 1.0f / 60.0f;  // Asumiendo 60 FPS

    for (size_t i = 0; i < particles.x.size(); ++i) {
        particles.x[i] += particles.vx[i] * dt;
        particles.y[i] += particles.vy[i] * dt;
        particles.z[i] += particles.vz[i] * dt;

        if (particles.x[i] < -HALF_WIDTH || particles.x[i] > HALF_WIDTH) {
            particles.vx[i] = -particles.vx[i];

            if (particles.x[i] < -HALF_WIDTH) particles.x[i] = -HALF_WIDTH;
            else if (particles.x[i] > HALF_WIDTH) particles.x[i] = HALF_WIDTH;
        }


        if (particles.y[i] < -HALF_HEIGHT || particles.y[i] > HALF_HEIGHT) {
            particles.vy[i] = -particles.vy[i];

            if (particles.y[i] < -HALF_HEIGHT) particles.y[i] = -HALF_HEIGHT;
            else if (particles.y[i] > HALF_HEIGHT) particles.y[i] = HALF_HEIGHT;
        }


        if (particles.z[i] < -HALF_DEPTH || particles.z[i] > HALF_DEPTH) {
            particles.vz[i] = -particles.vz[i];

            if (particles.z[i] < -HALF_DEPTH) particles.z[i] = -HALF_DEPTH;
            else if (particles.z[i] > HALF_DEPTH) particles.z[i] = HALF_DEPTH;
        }
    }
}
