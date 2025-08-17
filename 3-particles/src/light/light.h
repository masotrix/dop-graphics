#pragma once

#include <SFML/Graphics.hpp>
#include <camera_data.h>
#include <light_data.h>

Light create_light(float x, float y, float radius, sf::Color color);

void move_light(Light& light, Camera& camera);

void render_light(Light& light, Camera& camera);
