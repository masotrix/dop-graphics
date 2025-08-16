#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

constexpr size_t N = 10'000'000; // número de partículas
constexpr int ITER = 10;         // iteraciones de actualización
constexpr int CHUNK_SIZE = 4;    // para AoSoA

// ---------------- AoS ----------------
struct ParticleAoS {
    float x, y, z;
    float velocity;
};

// ---------------- AoSoA ----------------
struct ParticleChunk {
    float x[CHUNK_SIZE];
    float y[CHUNK_SIZE];
    float z[CHUNK_SIZE];
    float velocity[CHUNK_SIZE];
};

void update_aos(std::vector<ParticleAoS>& particles) {
    for (int iter = 0; iter < ITER; ++iter) {
        for (size_t i = 0; i < particles.size(); ++i) {
            particles[i].x += particles[i].velocity;
            particles[i].y += particles[i].velocity;
            particles[i].z += particles[i].velocity;
        }
    }
}

void update_soa(std::vector<float>& x, std::vector<float>& y, std::vector<float>& z, std::vector<float>& velocity) {
    for (int iter = 0; iter < ITER; ++iter) {
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] += velocity[i];
            y[i] += velocity[i];
            z[i] += velocity[i];
        }
    }
}

void update_aosoa(std::vector<ParticleChunk>& chunks) {
    for (int iter = 0; iter < ITER; ++iter) {
        for (size_t c = 0; c < chunks.size(); ++c) {
            for (int i = 0; i < CHUNK_SIZE; ++i) {
                chunks[c].x[i] += chunks[c].velocity[i];
                chunks[c].y[i] += chunks[c].velocity[i];
                chunks[c].z[i] += chunks[c].velocity[i];
            }
        }
    }
}

int main() {
    // Inicialización AoS
    std::vector<ParticleAoS> particles_aos(N);
    for (auto& p : particles_aos) p.velocity = 1.0f;

    // Inicialización SoA
    std::vector<float> x_soa(N, 0.0f), y_soa(N, 0.0f), z_soa(N, 0.0f), velocity_soa(N, 1.0f);

    // Inicialización AoSoA
    size_t num_chunks = N / CHUNK_SIZE;
    std::vector<ParticleChunk> chunks(num_chunks);
    for (auto& c : chunks)
        for (int i = 0; i < CHUNK_SIZE; ++i)
            c.velocity[i] = 1.0f;

    // Medir AoS
    {
        auto start = std::chrono::high_resolution_clock::now();
        update_aos(particles_aos);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "AoS: " << elapsed.count() << " segundos\n";
    }

    // Medir SoA
    {
        auto start = std::chrono::high_resolution_clock::now();
        update_soa(x_soa, y_soa, z_soa, velocity_soa);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "SoA: " << elapsed.count() << " segundos\n";
    }

    // Medir AoSoA
    {
        auto start = std::chrono::high_resolution_clock::now();
        update_aosoa(chunks);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "AoSoA: " << elapsed.count() << " segundos\n";
    }

    return 0;
}
