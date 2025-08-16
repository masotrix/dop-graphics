#pragma once

#include <SFML/Graphics.hpp>
#include <string>

void create_camera(sf::RenderWindow &camera, const std::string& DB_PATH);

void poll_event(sf::RenderWindow& camera);
