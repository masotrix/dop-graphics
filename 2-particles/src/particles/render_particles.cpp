#include <particles.h>

#include <SFML/Graphics.hpp>
#include <cmath>

void render_particles(Particles& particles, sf::RenderWindow& camera, const Light& light) {

    const float radius = 3.0f;

    camera.clear(sf::Color::Black);

    for (size_t i = 0; i < particles.x.size(); ++i) {

        sf::CircleShape point(radius);
        point.setPosition(particles.x[i], particles.y[i]);

        float dx = particles.x[i] - light.position.x;
        float dy = particles.y[i] - light.position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        float intensity = 1.f - (dist / light.radius);
        if (intensity < 0.f) intensity = 0.f;

        sf::Color base = particles.colors[i];
        sf::Color lit(
            static_cast<sf::Uint8>(base.r * intensity),
            static_cast<sf::Uint8>(base.g * intensity),
            static_cast<sf::Uint8>(base.b * intensity),
            base.a
        );

        point.setFillColor(lit);
        camera.draw(point);
    }
}
