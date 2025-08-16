#include <SFML/Graphics.hpp>

#include "positions.h"
#include "movement_system.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Random Points Animation");
    window.setFramerateLimit(60);

    // Inicializar posiciones
    Positions positions;
    for (int i = 0; i < 50; ++i) {
        positions.x.push_back(400.0f + i * 5.0f); // dispersión inicial
        positions.y.push_back(300.0f);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Actualizar posiciones aleatoriamente
        update_positions_random(positions, 2.0f);

        // Dibujar
        window.clear(sf::Color::Black);
        for (size_t i = 0; i < positions.x.size(); ++i) {
            sf::CircleShape point(3.0f);
            point.setFillColor(sf::Color::White);
            point.setPosition(positions.x[i], positions.y[i]);
            window.draw(point);
        }
        window.display();
    }

    return 0;
}

