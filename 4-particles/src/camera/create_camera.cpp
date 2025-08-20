#include <camera.h>

#include <SFML/Graphics.hpp>
#include <sqlite3.h>
#include <string>
#include <utility>
#include <iostream>

Camera create_camera(const std::string& DB_PATH) {

    Camera camera;
    camera.window = std::make_unique<sf::RenderWindow>();

    sqlite3* db;
    sqlite3_open(DB_PATH.c_str(), &db);

    unsigned int W_XINI = 0, W_YINI = 0;
    unsigned int W_WIDTH = 800, W_HEIGHT = 600;

    // Consulta para leer todos los valores
    const char* sql = "SELECT x, y, z, yaw, pitch, W_XINI, W_YINI, W_WIDTH, "
        "W_HEIGHT FROM camera WHERE id = 1;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            camera.x = static_cast<float>(sqlite3_column_int(stmt, 0));
            camera.y = static_cast<float>(sqlite3_column_int(stmt, 1));
            camera.z = static_cast<float>(sqlite3_column_int(stmt, 2));

            camera.yaw = static_cast<float>(sqlite3_column_int(stmt, 3));
            camera.pitch = static_cast<float>(sqlite3_column_int(stmt, 4));

            W_XINI = static_cast<unsigned int>(sqlite3_column_int(stmt, 5));
            W_YINI = static_cast<unsigned int>(sqlite3_column_int(stmt, 6));

            W_WIDTH = static_cast<unsigned int>(sqlite3_column_int(stmt, 7));
            W_HEIGHT = static_cast<unsigned int>(sqlite3_column_int(stmt, 8));
        }
    } else {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    camera.window->create(sf::VideoMode(W_WIDTH, W_HEIGHT), "Particle Animation (SQLite)");
    camera.window->setPosition(sf::Vector2i(W_XINI, W_YINI));

    camera.window->setFramerateLimit(60);

    return camera;
}
