#include <camera.h>

#include <SFML/Graphics.hpp>
#include <sqlite3.h>
#include <string>
#include <utility>
#include <iostream>

void create_camera(sf::RenderWindow& camera, const std::string& DB_PATH) {
    int x = 0;
    int y = 0;
    unsigned int width = 800;
    unsigned int height = 600;

    sqlite3* db;
    sqlite3_open(DB_PATH.c_str(), &db);

    // Consulta para leer todos los valores
    const char* sql = "SELECT x, y, width, height FROM camera WHERE id = 1;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            x = sqlite3_column_int(stmt, 0);
            y = sqlite3_column_int(stmt, 1);
            width = static_cast<unsigned int>(sqlite3_column_int(stmt, 2));
            height = static_cast<unsigned int>(sqlite3_column_int(stmt, 3));
        }
    } else {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Crear ventana con el tamaño guardado
    camera.create(sf::VideoMode(width, height), "Particle Animation (SQLite)");

    // Mover ventana a la posición guardada
    camera.setPosition(sf::Vector2i(x, y));

    camera.setFramerateLimit(60);
}
