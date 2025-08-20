#include <SFML/Graphics.hpp>

#include <camera.h>

void poll_event(Camera& camera) {
    sf::Event event;

    // Velocidad de movimiento de la cámara
    float moveSpeed = 5.0f; // Ajusta este valor según necesites

    // Manejo de eventos discretos (presionar/soltar teclas)
    while (camera.window->pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q))
            camera.window->close();
    }

    // Manejo de teclas mantenidas presionadas (movimiento continuo)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        camera.z += moveSpeed; // Acercar / mover hacia adelante
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        camera.z -= moveSpeed; // Alejar / mover hacia atrás
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        camera.x -= moveSpeed; // Mover hacia la izquierda
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        camera.x += moveSpeed; // Mover hacia la derecha
    }

    // Opcional: Control de zoom con Q/E
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        camera.y += moveSpeed; // Mover hacia la derecha
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        camera.y -= moveSpeed; // Mover hacia la derecha
    }

    return;
}
