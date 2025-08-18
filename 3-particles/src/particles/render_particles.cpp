// src/particles/render_particles.cpp
#include <particles.h>

#include <SFML/Graphics.hpp>
#include <cmath>

void render_particles(Particles& particles, Camera& camera, const Light& light) {

    const float base_radius = 3.0f;
    const float focal_length = 500.0f;

    for (size_t i = 0; i < particles.x.size(); ++i) {

        float x = particles.x[i];
        float y = particles.y[i];
        float z = particles.z[i];

        if (z <= 1.f) continue;

        float screenX = (x * focal_length / z) + camera.window->getSize().x / 2.f;
        float screenY = (y * focal_length / z) + camera.window->getSize().y / 2.f;

        float radius = base_radius * (focal_length / z);

        sf::CircleShape point(radius);
        point.setPosition(screenX, screenY);

        float dx = x - light.position.x;
        float dy = y - light.position.y;
        float dz = z - light.position.z;
        float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

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
        camera.window->draw(point);
    }
}
