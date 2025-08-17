#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include <camera_data.h>

Camera create_camera(const std::string& DB_PATH);

void poll_event(Camera& camera);
