#include <particles.h>

#include <SFML/Graphics.hpp>
#include <sqlite3.h>
#include <string>

Particles create_particles(const std::string& DB_PATH) {

    Particles particles;

    sqlite3* db;
    sqlite3_open(DB_PATH.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT x, y, z, vx, vy, vz, r, g, b FROM particles;",
        -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        particles.x.push_back(static_cast<float>(sqlite3_column_double(stmt, 0)));
        particles.y.push_back(static_cast<float>(sqlite3_column_double(stmt, 1)));
        particles.z.push_back(static_cast<float>(sqlite3_column_double(stmt, 2)));

        particles.vx.push_back(static_cast<float>(sqlite3_column_double(stmt, 3)));
        particles.vy.push_back(static_cast<float>(sqlite3_column_double(stmt, 4)));
        particles.vz.push_back(static_cast<float>(sqlite3_column_double(stmt, 5)));

        particles.colors.emplace_back(
            sqlite3_column_int(stmt, 6),
            sqlite3_column_int(stmt, 7),
            sqlite3_column_int(stmt, 8)
        );
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return particles;
}
