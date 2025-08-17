#include <particles.h>

#include <SFML/Graphics.hpp>
#include <sqlite3.h>
#include <string>

Particles create_particles(const std::string& DB_PATH) {

    Particles particles;

    sqlite3* db;
    sqlite3_open(DB_PATH.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT x, y, vx, vy, r, g, b FROM particles;", -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        particles.x.push_back(static_cast<float>(sqlite3_column_double(stmt, 0)));
        particles.y.push_back(static_cast<float>(sqlite3_column_double(stmt, 1)));
        particles.vx.push_back(static_cast<float>(sqlite3_column_double(stmt, 2)));
        particles.vy.push_back(static_cast<float>(sqlite3_column_double(stmt, 3)));
        particles.colors.emplace_back(
            sqlite3_column_int(stmt, 4),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6)
        );
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return particles;
}
