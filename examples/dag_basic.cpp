#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    const int n = 7;
    const auto edges = cp_stress_gen::Graph(n).edges(9).directed().dag().no_multi_edges().build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(n, edges.size());
    out.edges(edges);
    return 0;
}

