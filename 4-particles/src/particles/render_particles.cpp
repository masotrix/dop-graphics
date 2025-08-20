// src/particles/render_particles.cpp
#include <particles.h>

#include <SFML/Graphics.hpp>
#include <cmath>

void render_particles(Particles& particles, Camera& camera, const Light& light) {

    const float base_radius = 3.0f;
    const float focal_length = 500.0f;

    float cosYaw = std::cos(camera.yaw);
    float sinYaw = std::sin(camera.yaw);
    float cosPitch = std::cos(camera.pitch);
    float sinPitch = std::sin(camera.pitch);

    for (size_t i = 0; i < particles.x.size(); ++i) {

        float relX = particles.x[i] - camera.x;
        float relY = particles.y[i] - camera.y;
        float relZ = particles.z[i] - camera.z;

        float xz = cosYaw * relX - sinYaw * relZ;
        float zz = sinYaw * relX + cosYaw * relZ;

        float yz = cosPitch * relY - sinPitch * zz;
        float zz2 = sinPitch * relY + cosPitch * zz;

        if (zz2 <= 1.f) continue;

        float screenX = (xz * focal_length / zz2) + camera.window->getSize().x / 2.f;
        float screenY = (yz * focal_length / zz2) + camera.window->getSize().y / 2.f;

        float radius = base_radius * (focal_length / zz2);

        sf::CircleShape point(radius);
        point.setPosition(screenX, screenY);

        float dx = relX - light.position.x;
        float dy = relY - light.position.y;
        float dz = relZ - light.position.z;
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
