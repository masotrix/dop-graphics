#include <SFML/Graphics.hpp>

#include <particles.h>

void move_particles(Particles& particles, sf::RenderWindow& camera) {

    sf::Vector2u size = camera.getSize();
    unsigned int width = size.x;
    unsigned int height = size.y;

    for (size_t i = 0; i < particles.x.size(); ++i) {
        particles.x[i] += particles.vx[i];

        if (particles.x[i] < 0 || particles.x[i] > width)
            particles.vx[i] = -particles.vx[i];

        particles.y[i] += particles.vy[i];

        if (particles.y[i] < 0 || particles.y[i] > height)
            particles.vy[i] = -particles.vy[i];
    }
}
