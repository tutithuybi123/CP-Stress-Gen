#include "cp_stress_gen.hpp"

#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    cp_stress_gen::core::Options opt(argc, argv);
    if (opt.flag("help")) {
        std::cout << "usage: polygon_generator --n 20 --m 30 --seed 1 --mode graph\n";
        return 0;
    }

    const int n = std::max(1, opt.get<int>("n", 20));
    const int default_m = std::max(0, n - 1);
    const int m = opt.get<int>("m", default_m);
    const long long seed = opt.get<long long>("seed", 1);
    const std::string mode = opt.get<std::string>("mode", "graph");
    cp_stress_gen::core::require(m >= 0, "m must be non-negative");

    cp_stress_gen::core::Random rng(static_cast<unsigned long long>(seed));
    std::vector<cp_stress_gen::Graph::Edge> edges;

    if (mode == "dag") {
        edges = cp_stress_gen::Graph(static_cast<std::size_t>(n))
            .edges(static_cast<std::size_t>(m))
            .dag()
            .no_multi_edges()
            .build(rng);
    } else if (mode == "bipartite") {
        const std::size_t left = static_cast<std::size_t>(std::max(1, n / 2));
        edges = cp_stress_gen::Graph(static_cast<std::size_t>(n))
            .edges(static_cast<std::size_t>(m))
            .bipartite(left)
            .no_multi_edges()
            .build(rng);
    } else {
        edges = cp_stress_gen::Graph(static_cast<std::size_t>(n))
            .edges(static_cast<std::size_t>(m))
            .sparse_connected()
            .shuffle()
            .build(rng);
    }

    cp_stress_gen::core::Printer out(std::cout);
    out.line(n, edges.size());
    out.edges(edges);
    return 0;
}
