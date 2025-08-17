#include <SFML/Graphics.hpp>
#include <sqlite3.h>
#include <random>
#include <iostream>
#include <vector>
#include <string>

#include <light.h>
#include <particles.h>
#include <database.h>
#include <camera.h>

const std::string DB_PATH = "game.db";

int main() {

    seed_database(DB_PATH);

    sf::RenderWindow camera; create_camera(camera, DB_PATH);

    Particles particles = create_particles(DB_PATH);

    Light light = create_light(400, 300, 150.f, sf::Color(255, 255, 200));

    while (camera.isOpen()) {

        poll_event(camera);

        move_particles(particles, camera);
        move_light(light, camera);

        render_particles(particles, camera, light);
        //render_light(light, camera);

        camera.display();
    }

    return 0;
}
