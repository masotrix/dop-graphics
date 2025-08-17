#pragma once

#include <SFML/Graphics.hpp>

#include <light_data.h>

Light create_light(float x, float y, float radius, sf::Color color);

void move_light(Light& light, sf::RenderWindow& camera);

void render_light(Light& light, sf::RenderWindow& camera);
