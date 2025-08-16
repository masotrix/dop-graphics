#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

constexpr size_t N = 10'000'000; // número de partículas
constexpr int ITER = 10;         // iteraciones por benchmark
constexpr int RUNS = 5;          // número de repeticiones para estadística

// ---------------- AoS ----------------
struct ParticleAoS {
    float x, y, z;
    float velocity;
};

// ---------------- AoSoA genérico ----------------
template<int CHUNK_SIZE>
struct alignas(64) ParticleChunk {
    float x[CHUNK_SIZE];
    float y[CHUNK_SIZE];
    float z[CHUNK_SIZE];
    float velocity[CHUNK_SIZE];
};

// ---------------- Update functions ----------------
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

template<int CHUNK_SIZE>
void update_aosoa(std::vector<ParticleChunk<CHUNK_SIZE>>& chunks) {
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

// ---------------- Benchmark helper ----------------
template<typename Func>
double benchmark(Func&& func) {
    // Warm-up
    func();

    std::vector<double> times;
    for (int run = 0; run < RUNS; ++run) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
    }
    std::sort(times.begin(), times.end());
    return times[times.size()/2]; // mediana
}

int main() {
    // AoS setup
    std::vector<ParticleAoS> particles_aos(N);
    for (auto& p : particles_aos) p.velocity = 1.0f;

    // SoA setup
    std::vector<float> x_soa(N, 0.0f), y_soa(N, 0.0f), z_soa(N, 0.0f), velocity_soa(N, 1.0f);

    // AoSoA setups (varios tamaños de chunk)
    std::vector<int> chunk_sizes = {4, 8, 16, 32};

    std::cout << "Benchmark con N=" << N << ", ITER=" << ITER << ", RUNS=" << RUNS << "\n\n";

    double time_aos = benchmark([&] { update_aos(particles_aos); });
    std::cout << "AoS: " << time_aos << " segundos\n";

    double time_soa = benchmark([&] { update_soa(x_soa, y_soa, z_soa, velocity_soa); });
    std::cout << "SoA: " << time_soa << " segundos\n";

    for (int cs : chunk_sizes) {
        if (cs == 4) {
            std::vector<ParticleChunk<4>> chunks(N/cs);
            for (auto& c : chunks) for (int i = 0; i < cs; ++i) c.velocity[i] = 1.0f;
            double t = benchmark([&] { update_aosoa(chunks); });
            std::cout << "AoSoA (chunk=" << cs << "): " << t << " segundos\n";
        }
        if (cs == 8) {
            std::vector<ParticleChunk<8>> chunks(N/cs);
            for (auto& c : chunks) for (int i = 0; i < cs; ++i) c.velocity[i] = 1.0f;
            double t = benchmark([&] { update_aosoa(chunks); });
            std::cout << "AoSoA (chunk=" << cs << "): " << t << " segundos\n";
        }
        if (cs == 16) {
            std::vector<ParticleChunk<16>> chunks(N/cs);
            for (auto& c : chunks) for (int i = 0; i < cs; ++i) c.velocity[i] = 1.0f;
            double t = benchmark([&] { update_aosoa(chunks); });
            std::cout << "AoSoA (chunk=" << cs << "): " << t << " segundos\n";
        }
        if (cs == 32) {
            std::vector<ParticleChunk<32>> chunks(N/cs);
            for (auto& c : chunks) for (int i = 0; i < cs; ++i) c.velocity[i] = 1.0f;
            double t = benchmark([&] { update_aosoa(chunks); });
            std::cout << "AoSoA (chunk=" << cs << "): " << t << " segundos\n";
        }
    }

    return 0;
}
