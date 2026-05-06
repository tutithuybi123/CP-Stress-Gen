#include "cp_stress_gen.hpp"

#include <chrono>
#include <iostream>

int main(int argc, char** argv) {
    cp_stress_gen::core::Options opt(argc, argv);
    const int n = opt.get<int>("n", 1000);
    const int m = opt.get<int>("m", 2000);
    const long long seed = opt.get<long long>("seed", 1);
    cp_stress_gen::core::require(n > 0, "n must be positive");
    cp_stress_gen::core::require(m >= n - 1, "m must be at least n - 1 for sparse_connected");

    cp_stress_gen::core::Random rng(static_cast<unsigned long long>(seed));
    const auto start = std::chrono::high_resolution_clock::now();
    const auto edges = cp_stress_gen::Graph(static_cast<std::size_t>(n))
        .edges(static_cast<std::size_t>(m))
        .sparse_connected()
        .shuffle()
        .build(rng);
    const auto finish = std::chrono::high_resolution_clock::now();

    long long checksum = 0;
    for (const auto& edge : edges) {
        checksum ^= static_cast<long long>(edge.u) * 1000003LL + edge.v;
    }

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    cp_stress_gen::core::Printer out(std::cout);
    out.line("graph_ms", elapsed);
    out.line("n_m", n, edges.size());
    out.line("checksum", checksum);
    return 0;
}
