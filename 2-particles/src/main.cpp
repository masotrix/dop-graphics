#include <SFML/Graphics.hpp>
#include <sqlite3.h>
#include <random>
#include <iostream>
#include <vector>
#include <string>

#include <particles.h>
#include <database.h>
#include <camera.h>

const std::string DB_PATH = "game.db";

int main() {

    seed_database(DB_PATH);

    sf::RenderWindow camera; create_camera(camera, DB_PATH);

    Particles particles = create_particles(DB_PATH);

    while (camera.isOpen()) {

        poll_event(camera);

        move_particles(particles, camera);

        render_particles(particles, camera);
    }

    return 0;
}
