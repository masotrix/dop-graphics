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

    int wx = 0, wy = 0;
    unsigned int width = 800, height = 600;

    // Consulta para leer todos los valores
    const char* sql = "SELECT x, y, z, wx, wy, width, height FROM camera WHERE id = 1;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            camera.x = sqlite3_column_int(stmt, 0);
            camera.y = sqlite3_column_int(stmt, 1);
            camera.z = sqlite3_column_int(stmt, 2);
            wx = sqlite3_column_int(stmt, 3);
            wy = sqlite3_column_int(stmt, 4);
            width = static_cast<unsigned int>(sqlite3_column_int(stmt, 5));
            height = static_cast<unsigned int>(sqlite3_column_int(stmt, 6));
        }
    } else {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    camera.window->create(sf::VideoMode(width, height), "Particle Animation (SQLite)");
    camera.window->setPosition(sf::Vector2i(wx, wy));

    camera.window->setFramerateLimit(60);

    return camera;
}
