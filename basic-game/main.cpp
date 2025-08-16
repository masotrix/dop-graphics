#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

// Constantes
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PLAYER_WIDTH = 50;
const float PLAYER_HEIGHT = 20;
const float OBSTACLE_WIDTH = 50;
const float OBSTACLE_HEIGHT = 20;
const float PLAYER_SPEED = 300.0f;
const float OBSTACLE_SPEED = 200.0f;
const int MAX_OBSTACLES = 100;

// Data-Oriented Structures
struct Position {
    float x[MAX_OBSTACLES];
    float y[MAX_OBSTACLES];
};

struct ActiveFlags {
    bool active[MAX_OBSTACLES];
};

// Inicializa obstáculos
void spawnObstacle(Position& pos, ActiveFlags& flags) {
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (!flags.active[i]) {
            pos.x[i] = static_cast<float>(rand() % (WINDOW_WIDTH - (int)OBSTACLE_WIDTH));
            pos.y[i] = -OBSTACLE_HEIGHT;
            flags.active[i] = true;
            break;
        }
    }
}

// Actualiza posiciones de obstáculos
void updateObstacles(Position& pos, ActiveFlags& flags, float dt) {
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (flags.active[i]) {
            pos.y[i] += OBSTACLE_SPEED * dt;
            if (pos.y[i] > WINDOW_HEIGHT) {
                flags.active[i] = false;
            }
        }
    }
}

// Dibuja obstáculos
void renderObstacles(sf::RenderWindow& window, Position& pos, ActiveFlags& flags) {
    sf::RectangleShape shape(sf::Vector2f(OBSTACLE_WIDTH, OBSTACLE_HEIGHT));
    shape.setFillColor(sf::Color::Red);
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (flags.active[i]) {
            shape.setPosition(pos.x[i], pos.y[i]);
            window.draw(shape);
        }
    }
}

// Verifica colisión entre jugador y obstáculos
bool checkCollision(float playerX, float playerY, Position& pos, ActiveFlags& flags) {
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (flags.active[i]) {
            if (playerX < pos.x[i] + OBSTACLE_WIDTH &&
                playerX + PLAYER_WIDTH > pos.x[i] &&
                playerY < pos.y[i] + OBSTACLE_HEIGHT &&
                playerY + PLAYER_HEIGHT > pos.y[i]) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Data-Oriented Game");
    window.setFramerateLimit(60);

    // Jugador
    float playerX = WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2;
    float playerY = WINDOW_HEIGHT - PLAYER_HEIGHT - 10;
    sf::RectangleShape playerShape(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT));
    playerShape.setFillColor(sf::Color::Green);

    // Obstáculos
    Position obstaclePositions{};
    ActiveFlags obstacleFlags{};

    sf::Clock clock;
    float spawnTimer = 0.0f;
    bool gameOver = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!gameOver) {
            // Movimiento del jugador
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playerX > 0) {
                playerX -= PLAYER_SPEED * dt;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerX + PLAYER_WIDTH < WINDOW_WIDTH) {
                playerX += PLAYER_SPEED * dt;
            }

            // Spawnea obstáculos cada 1 segundo
            spawnTimer += dt;
            if (spawnTimer >= 1.0f) {
                spawnObstacle(obstaclePositions, obstacleFlags);
                spawnTimer = 0.0f;
            }

            // Actualiza obstáculos
            updateObstacles(obstaclePositions, obstacleFlags, dt);

            // Colisiones
            if (checkCollision(playerX, playerY, obstaclePositions, obstacleFlags)) {
                gameOver = true;
            }
        }

        // Render
        window.clear();
        playerShape.setPosition(playerX, playerY);
        window.draw(playerShape);
        renderObstacles(window, obstaclePositions, obstacleFlags);

        if (gameOver) {
            sf::Font font;

            if (font.loadFromFile("assets/monserrat-variable.ttf")) {
                sf::Text text("Game Over", font, 50);
                text.setFillColor(sf::Color::White);
                text.setPosition(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 - 30);
                window.draw(text);
            }
        }

        window.display();
    }

    return 0;
}

