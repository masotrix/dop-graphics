#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct Particles {
    std::vector<float> x, y, z, vx, vy, vz;
    std::vector<sf::Color> colors;
};
