#include <SFML/Graphics.hpp>

#include <camera.h>

void poll_event(Camera& camera) {

    sf::Event event;

    while (camera.window->pollEvent(event)) {

        if (event.type == sf::Event::Closed ||

            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q))

            camera.window->close();
    }

    return;
}
