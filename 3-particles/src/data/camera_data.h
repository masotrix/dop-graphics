#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

struct Camera {
    float x, y, z;
    float fov;
    std::unique_ptr<sf::RenderWindow> window;
};
