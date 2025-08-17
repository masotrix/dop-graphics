#include <light.h>

#include <SFML/Graphics.hpp>

void move_light(Light& light, Camera& camera) {
    sf::Vector2i mouse = sf::Mouse::getPosition(*(camera.window));
    light.position = (sf::Vector2f)mouse;
    light.shape.setPosition(light.position);
}
