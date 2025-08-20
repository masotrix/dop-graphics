#pragma once

#include <SFML/Graphics.hpp>

struct Light {
    sf::CircleShape shape;
    sf::Vector3f position;
    float radius;
    sf::Color color;
};
