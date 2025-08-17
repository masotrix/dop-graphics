#pragma once

#include <SFML/Graphics.hpp>

struct Light {
    sf::CircleShape shape;
    sf::Vector2f position;
    float radius;
    sf::Color color;
};
