#include <light.h>

#include <SFML/Graphics.hpp>

void move_light(Light& light, Camera& camera) {
    sf::Vector2i mouse = sf::Mouse::getPosition(*(camera.window));

    float x = mouse.x - camera.window->getSize().x / 2.f;
    float y = mouse.y - camera.window->getSize().y / 2.f;
    float z = 250.f;

    light.position = { x, y, z };

    light.shape.setPosition(mouse.x, mouse.y);
}
