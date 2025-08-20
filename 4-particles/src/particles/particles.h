#pragma once

#include <string>
#include <vector>

#include <camera_data.h>
#include <particles_data.h>
#include <light_data.h>


Particles create_particles(const std::string& DB_PATH);

void move_particles(Particles& particles, Camera& camera);

void render_particles(Particles& particles, Camera& camera, const Light& light);
