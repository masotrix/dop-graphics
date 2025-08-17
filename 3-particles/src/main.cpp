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

    Camera camera = create_camera(DB_PATH);

    Particles particles = create_particles(DB_PATH);

    Light light = create_light(400, 300, 150.f, sf::Color(255, 255, 200));

    while (camera.window->isOpen()) {

        poll_event(camera);

        move_particles(particles, camera);
        move_light(light, camera);

        render_particles(particles, camera, light);

        camera.window->display();
    }

    return 0;
}
