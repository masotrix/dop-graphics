#include <SFML/Graphics.hpp>

#include <particles.h>

void render_particles(Particles& particles, sf::RenderWindow& camera) {

    const float radius = 3.0f;

    camera.clear(sf::Color::Black);

    for (size_t i = 0; i < particles.x.size(); ++i) {

        sf::CircleShape point(radius);
        point.setFillColor(particles.colors[i]);
        point.setPosition(particles.x[i], particles.y[i]);
        camera.draw(point);
    }

    camera.display();
}
