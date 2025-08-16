#include <SFML/Graphics.hpp>

#include <camera.h>

void poll_event(sf::RenderWindow& window) {

    sf::Event event;

    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed ||

            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q))

            window.close();
    }

    return;
}
