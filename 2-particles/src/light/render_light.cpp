#include <light.h>

#include <SFML/Graphics.hpp>

void render_light(Light& light, sf::RenderWindow& camera) {
    camera.draw(light.shape, sf::BlendAdd);
}

