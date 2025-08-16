// bench_aos_soa_aosoa.cpp
#include <immintrin.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <malloc.h>

using hr_clock = std::chrono::high_resolution_clock;
using fsec = std::chrono::duration<double>;

// Problem size
constexpr size_t N = 8'000'000; // ajusta según memoria/CPU
constexpr float DT = 0.016f;    // timestep
constexpr int REPS = 10;        // repeticiones para promediar

// ---------------- AoS ----------------
struct Particle {
    float px, py;
    float vx, vy;
};

void init_aos(std::vector<Particle> &a) {
    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> d(-1.0f, 1.0f);
    for (size_t i = 0; i < a.size(); ++i) {
        a[i].px = d(rng); a[i].py = d(rng);
        a[i].vx = d(rng); a[i].vy = d(rng);
    }
}

double update_aos(std::vector<Particle> &a, int reps) {
    // warm
    for (int r=0;r<2;++r)
        for (auto &p : a) {
            p.px += p.vx * DT;
            p.py += p.vy * DT;
        }

    auto t0 = hr_clock::now();
    for (int rep = 0; rep < reps; ++rep) {
        for (auto &p : a) {
            p.px += p.vx * DT;
            p.py += p.vy * DT;
        }
    }
    auto t1 = hr_clock::now();
    return fsec(t1 - t0).count() / reps;
}

// ---------------- SoA (vectorizado AVX2) ----------------
struct SoA {
    float *px, *py;
    float *vx, *vy;
    SoA(size_t n) {
        /*
        px = static_cast<float*>(std::aligned_alloc(32, n * sizeof(float)));
        py = static_cast<float*>(std::aligned_alloc(32, n * sizeof(float)));
        vx = static_cast<float*>(std::aligned_alloc(32, n * sizeof(float)));
        vy = static_cast<float*>(std::aligned_alloc(32, n * sizeof(float)));
        */
        px = static_cast<float*>(_aligned_malloc(n * sizeof(float), 32));
        py = static_cast<float*>(_aligned_malloc(n * sizeof(float), 32));
        vx = static_cast<float*>(_aligned_malloc(n * sizeof(float), 32));
        vy = static_cast<float*>(_aligned_malloc(n * sizeof(float), 32));
    }
    ~SoA() {
        free(px); free(py); free(vx); free(vy);
    }
};

void init_soa(SoA &s, size_t n) {
    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> d(-1.0f, 1.0f);
    for (size_t i=0;i<n;++i) {
        s.px[i]=d(rng); s.py[i]=d(rng);
        s.vx[i]=d(rng); s.vy[i]=d(rng);
    }
}

double update_soa_avx2(SoA &s, size_t n, int reps) {
    constexpr size_t V = 8; // AVX2: 8 floats per __m256
    size_t i = 0;
    // warm
    for (int r=0;r<2;++r) {
        for (i=0;i+V<=n;i+=V) {
            __m256 px = _mm256_load_ps(&s.px[i]);
            __m256 vx = _mm256_load_ps(&s.vx[i]);
            __m256 add = _mm256_mul_ps(vx, _mm256_set1_ps(DT));
            px = _mm256_add_ps(px, add);
            _mm256_store_ps(&s.px[i], px);

            __m256 py = _mm256_load_ps(&s.py[i]);
            __m256 vy = _mm256_load_ps(&s.vy[i]);
            add = _mm256_mul_ps(vy, _mm256_set1_ps(DT));
            py = _mm256_add_ps(py, add);
            _mm256_store_ps(&s.py[i], py);
        }
        for (; i<n; ++i) {
            s.px[i] += s.vx[i] * DT;
            s.py[i] += s.vy[i] * DT;
        }
    }

    auto t0 = hr_clock::now();
    for (int rep=0; rep<reps; ++rep) {
        size_t ii = 0;
        for (; ii+V<=n; ii+=V) {
            __m256 px = _mm256_load_ps(&s.px[ii]);
            __m256 vx = _mm256_load_ps(&s.vx[ii]);
            __m256 add = _mm256_mul_ps(vx, _mm256_set1_ps(DT));
            px = _mm256_add_ps(px, add);
            _mm256_store_ps(&s.px[ii], px);

            __m256 py = _mm256_load_ps(&s.py[ii]);
            __m256 vy = _mm256_load_ps(&s.vy[ii]);
            add = _mm256_mul_ps(vy, _mm256_set1_ps(DT));
            py = _mm256_add_ps(py, add);
            _mm256_store_ps(&s.py[ii], py);
        }
        for (; ii<n; ++ii) {
            s.px[ii] += s.vx[ii] * DT;
            s.py[ii] += s.vy[ii] * DT;
        }
    }
    auto t1 = hr_clock::now();
    return fsec(t1 - t0).count() / reps;
}

// ---------------- AoSoA (bloques de 8 floats) ----------------
constexpr size_t BLOCK = 8;
struct Block {
    alignas(32) float px[BLOCK];
    alignas(32) float py[BLOCK];
    alignas(32) float vx[BLOCK];
    alignas(32) float vy[BLOCK];
};

struct AoSoA {
    std::vector<Block> blocks;
    size_t count;
    AoSoA(size_t n) {
        count = n;
        blocks.resize((n + BLOCK - 1) / BLOCK);
    }
};

void init_aosoa(AoSoA &a, size_t n) {
    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> d(-1.0f, 1.0f);
    for (size_t i=0;i<n;++i) {
        size_t b = i / BLOCK;
        size_t e = i % BLOCK;
        a.blocks[b].px[e] = d(rng);
        a.blocks[b].py[e] = d(rng);
        a.blocks[b].vx[e] = d(rng);
        a.blocks[b].vy[e] = d(rng);
    }
}

double update_aosoa_avx2(AoSoA &a, size_t n, int reps) {
    const size_t nb = a.blocks.size();
    // warm
    for (int r=0;r<2;++r) {
        for (size_t b=0;b<nb;++b) {
            __m256 px = _mm256_load_ps(a.blocks[b].px);
            __m256 vx = _mm256_load_ps(a.blocks[b].vx);
            px = _mm256_add_ps(px, _mm256_mul_ps(vx, _mm256_set1_ps(DT)));
            _mm256_store_ps(a.blocks[b].px, px);

            __m256 py = _mm256_load_ps(a.blocks[b].py);
            __m256 vy = _mm256_load_ps(a.blocks[b].vy);
            py = _mm256_add_ps(py, _mm256_mul_ps(vy, _mm256_set1_ps(DT)));
            _mm256_store_ps(a.blocks[b].py, py);
        }
    }

    auto t0 = hr_clock::now();
    for (int rep=0; rep<reps; ++rep) {
        for (size_t b=0;b<nb;++b) {
            // be careful with tail in last block
            size_t base = b * BLOCK;
            if (base + BLOCK <= n) {
                __m256 px = _mm256_load_ps(a.blocks[b].px);
                __m256 vx = _mm256_load_ps(a.blocks[b].vx);
                px = _mm256_add_ps(px, _mm256_mul_ps(vx, _mm256_set1_ps(DT)));
                _mm256_store_ps(a.blocks[b].px, px);

                __m256 py = _mm256_load_ps(a.blocks[b].py);
                __m256 vy = _mm256_load_ps(a.blocks[b].vy);
                py = _mm256_add_ps(py, _mm256_mul_ps(vy, _mm256_set1_ps(DT)));
                _mm256_store_ps(a.blocks[b].py, py);
            } else {
                // tail: scalar
                for (size_t e=0; e < BLOCK && base + e < n; ++e) {
                    a.blocks[b].px[e] += a.blocks[b].vx[e] * DT;
                    a.blocks[b].py[e] += a.blocks[b].vy[e] * DT;
                }
            }
        }
    }
    auto t1 = hr_clock::now();
    return fsec(t1 - t0).count() / reps;
}

// ---------------- main ----------------
int main() {
    std::cout << "N = " << N << "\n";
    // AoS
    std::vector<Particle> aos(N);
    init_aos(aos);
    double t_aos = update_aos(aos, REPS);
    std::cout << "AoS time (s): " << t_aos << "\n";

    // SoA
    SoA soa(N);
    init_soa(soa, N);
    double t_soa = update_soa_avx2(soa, N, REPS);
    std::cout << "SoA+AVX2 time (s): " << t_soa << "\n";

    // AoSoA
    AoSoA aosoa(N);
    init_aosoa(aosoa, N);
    double t_aosoa = update_aosoa_avx2(aosoa, N, REPS);
    std::cout << "AoSoA+AVX2 time (s): " << t_aosoa << "\n";

    return 0;
}

