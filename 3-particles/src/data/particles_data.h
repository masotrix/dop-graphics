#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct Particles {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> vx;
    std::vector<float> vy;
    std::vector<sf::Color> colors;
};
