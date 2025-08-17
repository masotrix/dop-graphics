#include <light.h>

#include <SFML/Graphics.hpp>

Light create_light(float x, float y, float radius, sf::Color color) {
    Light light;
    light.radius = radius;
    light.position = {x, y};
    light.color = color;

    light.shape.setRadius(radius);
    light.shape.setOrigin(radius, radius); // centro
    light.shape.setPosition(x, y);
    light.shape.setFillColor(sf::Color(color.r, color.g, color.b, 128)); // semi-transparente

    return light;
}

