// src/database/seed_database.cpp
#include <database.h>

#include <random>
#include <sqlite3.h>
#include <iostream>
#include <cassert>
#include <chrono>

bool seed_database(const std::string& DB_PATH) {

    const int NUM_PARTICLES = 12800;
    const float SPEED_MIN = -60.f, SPEED_MAX = 60.f;
    const float HALF_WIDTH = 1000.f;
    const float HALF_HEIGHT = 1000.f;
    const float HALF_DEPTH = 1000.f;

    const float CAMERA_XINI = 0, CAMERA_YINI = 0, CAMERA_ZINI = 0;
    const float CAMERA_YAW = 0, CAMERA_PITCH = 0;
    const unsigned int WINDOW_XINI = 0, WINDOW_YINI = 0;
    const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;


    sqlite3* db;
    if (sqlite3_open(DB_PATH.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error abriendo DB: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    const char* createTablesSQL = R"SQL(
        DROP TABLE IF EXISTS particles;
        DROP TABLE IF EXISTS camera;
        CREATE TABLE particles (
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
            yaw REAL NOT NULL,
            pitch REAL NOT NULL,
            W_XINI INTEGER NOT NULL,
            W_YINI INTEGER NOT NULL,
            W_WIDTH INTEGER NOT NULL,
            W_HEIGHT INTEGER NOT NULL
        );
    )SQL";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTablesSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creando tablas: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    // Insertar cámara inicial
    std::string cameraSQLInsert =
        "INSERT INTO camera (x, y, z, yaw, pitch, W_XINI, W_YINI, W_WIDTH, W_HEIGHT) VALUES (" +
        std::to_string(CAMERA_XINI) + "," +
        std::to_string(CAMERA_YINI) + "," +
        std::to_string(CAMERA_ZINI) + "," +

        std::to_string(CAMERA_YAW) + "," +
        std::to_string(CAMERA_PITCH) + "," +

        std::to_string(WINDOW_XINI) + "," +
        std::to_string(WINDOW_YINI) + "," +

        std::to_string(WINDOW_WIDTH) + "," +
        std::to_string(WINDOW_HEIGHT) + ");";

    sqlite3_exec(db, cameraSQLInsert.c_str(), nullptr, nullptr, nullptr);

    // Generador aleatorio
    //std::mt19937 rng(std::random_device{}());
    std::mt19937 rng(static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    ));

    std::uniform_real_distribution<float> distX(-HALF_WIDTH, HALF_WIDTH);
    std::uniform_real_distribution<float> distY(-HALF_HEIGHT, HALF_HEIGHT);
    std::uniform_real_distribution<float> distZ(-HALF_DEPTH, HALF_DEPTH);

    std::uniform_real_distribution<float> distVel(SPEED_MIN, SPEED_MAX);
    std::uniform_int_distribution<int> distColor(0, 255);

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        float x = distX(rng), y = distY(rng), z = distZ(rng);
        float vx = distVel(rng), vy = distVel(rng), vz = distVel(rng);
        int r = distColor(rng), g = distColor(rng), b = distColor(rng);

        std::string insertSQL = "INSERT INTO particles (x, y, z, vx, vy, vz, r, g, b) VALUES (" +
            std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," +
            std::to_string(vx) + "," + std::to_string(vy) + "," + std::to_string(vz) + "," +
            //std::to_string(vx) + "," + std::to_string(vy) + "," + std::to_string(0) + "," +
            std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ");";
        sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, nullptr);
    }
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

    sqlite3_close(db);
    return true;
}
