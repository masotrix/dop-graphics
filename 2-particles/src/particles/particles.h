#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

#include <particles_data.h>
#include <light_data.h>


Particles create_particles(const std::string& DB_PATH);

void move_particles(Particles& particles, sf::RenderWindow& camera);

void render_particles(Particles& particles, sf::RenderWindow& camera, const Light& light);
