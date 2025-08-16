#include <cmath>
#include <random>

#include <movement.h>

void update_positions(Positions& positions, float dx, float dy) {
    for (size_t i = 0; i < positions.x.size(); ++i) {
        positions.x[i] += dx;
        positions.y[i] += dy;
    }
}

void update_positions_random(Positions& positions, float range) {

    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-range, range);

    for (size_t i = 0; i < positions.x.size(); ++i) {
        positions.x[i] += dist(rng);
        positions.y[i] += dist(rng);
    }
}

