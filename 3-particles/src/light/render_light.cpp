#include <light.h>

#include <SFML/Graphics.hpp>

void render_light(Light& light, Camera& camera) {
    camera.window->draw(light.shape, sf::BlendAdd);
}

