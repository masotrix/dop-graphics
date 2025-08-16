#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <random>

#include "positions.h"
#include "movement_system.h"

int main() {
    
    std::ifstream file("assets/particles.json");
    nlohmann::json config;
    file >> config;

    const unsigned int WINDOW_WIDTH = config["window"]["width"];
    const unsigned int WINDOW_HEIGHT = config["window"]["height"];
    const int NUM_PARTICLES = config["particles"]["count"];
    const float RADIUS = config["particles"]["radius"];
    const float SPEED_MIN = config["particles"]["speed_min"];
    const float SPEED_MAX = config["particles"]["speed_max"];
    const bool RANDOM_COLOR = config["particles"]["color_random"];

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Animation");
    window.setFramerateLimit(60);

    // Inicialización de partículas
    Positions positions;
    std::vector<sf::Color> colors(NUM_PARTICLES);
    std::vector<float> vx(NUM_PARTICLES);
    std::vector<float> vy(NUM_PARTICLES);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> distX(0.f, WINDOW_WIDTH);
    std::uniform_real_distribution<float> distY(0.f, WINDOW_HEIGHT);
    std::uniform_real_distribution<float> distVel(SPEED_MIN, SPEED_MAX);
    std::uniform_int_distribution<int> distColor(0, 255);

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        positions.x.push_back(distX(rng));
        positions.y.push_back(distY(rng));
        vx[i] = distVel(rng);
        vy[i] = distVel(rng);
        colors[i] = sf::Color(distColor(rng), distColor(rng), distColor(rng));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q))
                window.close();
        }

        // Actualizar posiciones con velocidad y rebote
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            positions.x[i] += vx[i];
            positions.y[i] += vy[i];

            if (positions.x[i] < 0 || positions.x[i] > WINDOW_WIDTH) vx[i] = -vx[i];
            if (positions.y[i] < 0 || positions.y[i] > WINDOW_HEIGHT) vy[i] = -vy[i];
        }

        // Dibujar
        window.clear(sf::Color::Black);
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            sf::CircleShape point(RADIUS);
            point.setFillColor(colors[i]);
            point.setPosition(positions.x[i], positions.y[i]);
            window.draw(point);
        }
        window.display();
    }

    return 0;
}
