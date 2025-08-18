// src/particles/move_particles.cpp
#include <SFML/Graphics.hpp>

#include <particles.h>

void move_particles(Particles& particles, Camera& camera) {

    sf::Vector2u size = camera.window->getSize();
    const float width = static_cast<float>(size.x);
    const float height = static_cast<float>(size.y);
    const float depth = 1000.f;
    const float dt = 1.0f / 60.0f;  // Asumiendo 60 FPS

    for (size_t i = 0; i < particles.x.size(); ++i) {
        particles.x[i] += particles.vx[i] * dt;
        particles.y[i] += particles.vy[i] * dt;
        particles.z[i] += particles.vz[i] * dt;

        if (particles.x[i] < -width/2.f || particles.x[i] > width/2.f) {
            particles.vx[i] = -particles.vx[i];

            if (particles.x[i] < -width/2.f) particles.x[i] = -width/2.f;
            else if (particles.x[i] > width/2.f) particles.x[i] = width/2.f;
        }


        if (particles.y[i] < -height/2.f || particles.y[i] > height/2.f) {
            particles.vy[i] = -particles.vy[i];

            if (particles.y[i] < -height/2.f) particles.y[i] = -height/2.f;
            else if (particles.y[i] > height/2.f) particles.y[i] = height/2.f;
        }


        if (particles.z[i] < 1.f || particles.z[i] > depth) {
            particles.vz[i] = -particles.vz[i];

            if (particles.z[i] < 1.f) particles.z[i] = 1.f;
            else if (particles.z[i] > depth) particles.z[i] = depth;
        }
    }
}
