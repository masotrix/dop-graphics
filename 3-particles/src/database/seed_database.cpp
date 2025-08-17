// src/database/seed_database.cpp
#include <database.h>

#include <random>
#include <sqlite3.h>
#include <iostream>
#include <cassert>

bool seed_database(const std::string& DB_PATH) {

    const int NUM_PARTICLES = 200;
    const float SPEED_MIN = -2.0f, SPEED_MAX = 2.0f;
    const float CAMERA_XINI = 0, CAMERA_YINI = 0, CAMERA_ZINI = 500;
    const unsigned int WINDOW_XINI = 0, WINDOW_YINI = 0;
    const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600, WINDOW_DEPTH = 1000;

    sqlite3* db;
    if (sqlite3_open(DB_PATH.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error abriendo DB: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    const char* createTablesSQL = R"SQL(
        CREATE TABLE IF NOT EXISTS particles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            x REAL NOT NULL,
            y REAL NOT NULL,
            z REAL NOT NULL,
            vx REAL NOT NULL,
            vy REAL NOT NULL,
            vz REAL NOT NULL,
            r INTEGER NOT NULL,
            g INTEGER NOT NULL,
            b INTEGER NOT NULL
        );
        CREATE TABLE IF NOT EXISTS camera (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            x REAL NOT NULL,
            y REAL NOT NULL,
            z REAL NOT NULL,
            wx REAL NOT NULL,
            wy REAL NOT NULL,
            width INTEGER NOT NULL,
            height INTEGER NOT NULL,
            depth INTEGER NOT NULL
        );
    )SQL";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTablesSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creando tablas: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    // Limpiar camara previa
    sqlite3_exec(db, "DELETE FROM camera;", nullptr, nullptr, nullptr);

    // Insertar cámara inicial
    std::string cameraSQLInsert =
        "INSERT OR REPLACE INTO camera (id, x, y, z, wx, wy, width, height, depth) VALUES (1," +
        std::to_string(CAMERA_XINI) + "," +
        std::to_string(CAMERA_YINI) + "," +
        std::to_string(CAMERA_ZINI) + "," +

        std::to_string(WINDOW_XINI) + "," +
        std::to_string(WINDOW_YINI) + "," +

        std::to_string(WINDOW_WIDTH) + "," +
        std::to_string(WINDOW_HEIGHT) + "," +
        std::to_string(WINDOW_DEPTH) + ");";
    sqlite3_exec(db, cameraSQLInsert.c_str(), nullptr, nullptr, nullptr);

    // Generador aleatorio
    std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<float> distX(
        -static_cast<float>(WINDOW_WIDTH) / 2.f, static_cast<float>(WINDOW_WIDTH) / 2.f);

    std::uniform_real_distribution<float> distY(
        -static_cast<float>(WINDOW_HEIGHT) / 2.f, static_cast<float>(WINDOW_HEIGHT) / 2.f);

    std::uniform_real_distribution<float> distZ(
        1.f, static_cast<float>(WINDOW_DEPTH)); // ojo: z > 0

    std::uniform_real_distribution<float> distVel(SPEED_MIN, SPEED_MAX);
    std::uniform_int_distribution<int> distColor(0, 255);

    // Limpiar partículas previas
    sqlite3_exec(db, "DELETE FROM particles;", nullptr, nullptr, nullptr);

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        float x = distX(rng), y = distY(rng), z = distZ(rng);
        float vx = distVel(rng), vy = distVel(rng), vz = distVel(rng);
        int r = distColor(rng), g = distColor(rng), b = distColor(rng);

        std::string insertSQL = "INSERT INTO particles (x, y, z, vx, vy, vz, r, g, b) VALUES (" +
            std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," +
            std::to_string(vx) + "," + std::to_string(vy) + "," + std::to_string(vz) + "," +
            std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ");";
        sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, nullptr);
    }
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

    sqlite3_close(db);
    return true;
}
