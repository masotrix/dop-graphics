#include <SFML/Graphics.hpp>

#include <particles.h>

void move_particles(Particles& particles, Camera& camera) {

    sf::Vector2u size = camera.window->getSize();
    unsigned int width = size.x;
    unsigned int height = size.y;
    unsigned int depth = 1000;

    for (size_t i = 0; i < particles.x.size(); ++i) {
        particles.x[i] += particles.vx[i];

        if (particles.x[i] < -width/2 || particles.x[i] > width/2)
            particles.vx[i] = -particles.vx[i];

        particles.y[i] += particles.vy[i];

        if (particles.y[i] < -height/2 || particles.y[i] > height/2)
            particles.vy[i] = -particles.vy[i];

        particles.z[i] += particles.vz[i];

        if (particles.z[i] < 1 || particles.z[i] > depth)
            particles.vz[i] = -particles.vz[i];
    }
}
